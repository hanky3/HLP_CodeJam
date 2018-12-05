from collections import *
import json
import pprint
import struct
import re


class T32DataPacketError(Exception):
    def __init__(self, msg):
        self.msg = msg

    def __str__(self):
        return self.msg


class T32DataEncoder:
    @staticmethod
    def enc_bytestr(byte_val, fixed_size=0):
        byte_arr_value = bytearray(byte_val)
        if fixed_size > 0 :
            byte_arr_value = bytearray(fixed_size)
            for pos in range(min(len(byte_val), fixed_size)):
                byte_arr_value[pos] = byte_val[pos]

        return ''.join('%02X'%b for b in byte_arr_value)

    @staticmethod
    def enc_int(int_val, size=2, endian='big', to_bytestr=True):
        formatters = {1: 'B', 2: 'H', 4: 'L', 8: 'Q'}
        if size not in formatters:
            raise T32DataPacketError('[Encode Error][enc_int]Invalid Size : %d' % size)

        fmt = formatters[size]
        if int_val < 0:
            fmt = fmt.lower()
        if endian == 'big':
            fmt = '>' + fmt

        enc_val = struct.pack(fmt, int_val)
        if to_bytestr :
            enc_val = T32DataEncoder.enc_bytestr(enc_val)
        return enc_val

    @staticmethod
    def enc_float(float_val, size=4, endian='big', to_bytestr=True):
        formatters = {4: 'f', 8: 'd'}
        if size not in formatters:
            raise T32DataPacketError('[Encode Error][enc_float]Invalid Size : %d' % size)
        fmt = formatters[size]
        if endian == 'big':
            fmt = '>' + fmt

        enc_val = struct.pack(fmt, float_val)
        if to_bytestr :
            enc_val = T32DataEncoder.enc_bytestr(enc_val)
        return enc_val

    @staticmethod
    def enc_string(str_val, size=0, endian='big', to_bytestr=True):
        if size > len(str_val):
            raise T32DataPacketError('[Encode Error][enc_string]Invalid Size : %d' % size)
        enc_val = str_val.encode()
        if to_bytestr:
            enc_val = T32DataEncoder.enc_bytestr(enc_val)
        return enc_val


class T32DataDecoder:
    @staticmethod
    def decode_byte(data, size=0, encoding='hex', endian='big'):
        if size == 0:
            size = int(len(data)/2)

        end = (2 * size)
        if len(data) < end:
            raise T32DataPacketError("Decode Error : Length Mismatch : Data(%s), size(%d)" % (data, size))
        byte_val = bytes([int(data[pos:pos+2], 16) for pos in range(0, end, 2)])
        return byte_val

    @staticmethod
    def decode_int(data, size=4, encoding='hex', endian='big'):
        if encoding == 'plain':
            return int(data)

        if encoding == 'bytes':
            byte_value = data
        else :
            byte_value = T32DataDecoder.decode_byte(data, size)

        if endian == 'big':
            byte_value = list(reversed(byte_value[:size]))
        int_val = sum([byte_value[pos] << (8*pos) for pos in range(size)])
        return int_val

    @staticmethod
    def decode_float(data, size=4, encoding='hex', endian='big'):
        if encoding == 'plain':
            return float(data)

        if encoding == 'bytes':
            byte_value = data
        else:
            byte_value = T32DataDecoder.decode_byte(data, size)
            if endian == 'big':
                byte_value = bytearray(reversed(byte_value))

        return struct.unpack('f', byte_value[:size])[0]

    @staticmethod
    def decode_string(data, size=0, encoding='hex', type='utf-8'):
        if encoding == 'plain':
            return str(data[:size])
        if size == 0:
            size = int(len(data)/2)
        return T32DataDecoder.decode_byte(data, size).decode(type)


class T32DataPacket:
    def __init__(self, name, parameters):
        self.name = name
        self.parameters = parameters
        self.decode_func = None
        self.encode_func = None
        self.__init_decode_func__()
        self.__init_encode_func__()
        self.parameter_values = self.__init_param_values__(parameters)

    def __init_encode_func__(self):
        self.encode_func = defaultdict()
        self.encode_func['ByteArray'] = T32DataEncoder.enc_bytestr
        self.encode_func['String'] = T32DataEncoder.enc_string
        self.encode_func['Int'] = T32DataEncoder.enc_int
        self.encode_func['Float'] = T32DataEncoder.enc_float

    def __init_decode_func__(self):
        self.decode_func = defaultdict()
        self.decode_func['ByteArray'] = T32DataDecoder.decode_byte
        self.decode_func['String'] = T32DataDecoder.decode_string
        self.decode_func['Int'] = T32DataDecoder.decode_int
        self.decode_func['Float'] = T32DataDecoder.decode_float
        self.decode_func['Struct'] = self.decode

    def __init_param_values__(self, parameters):
        param_values = defaultdict()
        for name, param_config in parameters.items():
            param_values[name] = None
            if param_config['type'] == 'Struct':
                param_values[name] = [defaultdict()]*1
                param_values[name][0] = self.__init_param_values__(param_config['sub_parameters'])

        return param_values

    def __decode__(self, data_values, data_pos=0, data_sub_pos=0, parameters=defaultdict()):
        param_values = defaultdict()

        if len(data_values) <= data_pos:
            raise T32DataPacketError("[Decode Error]No remain data during decoding : curr_pos(%d), curr_params(%s)"
                                     % (data_pos, ''.join(name+',' for name in parameters)[:-1]))

        for name, meta_info in parameters.items():
            (param_type, data_size, encoding) = meta_info['type'], meta_info['size'], meta_info['encoding']
            if param_type not in self.decode_func:
                raise T32DataPacketError("[Decode Error]Invalid type(%s) - %s" % (type, name))
            if encoding not in ['plain', 'hex', 'hex_stream']:
                raise T32DataPacketError("[Decode Error]Invalid encoding type(%s) - %s" % (encoding, name))

            if param_type == 'Struct':
                arr_size_from = meta_info['array_size_from']
                array_size = meta_info['array_size']
                if arr_size_from in param_values:
                    array_size = param_values[arr_size_from]

                param_values[name] = [defaultdict()]*array_size
                for pos in range(array_size):
                    (data_pos, data_sub_pos, param_values[name][pos]) \
                        = self.__decode__(data_values, data_pos, data_sub_pos, meta_info['sub_parameters'])
                continue

            param_values[name] = self.decode_func[param_type](data_values[data_pos][data_sub_pos:], data_size, encoding=encoding)
            if encoding == 'hex_stream':
                data_sub_pos += (data_size*2)
                if data_sub_pos >= len(data_values[data_pos]):
                    (data_pos, data_sub_pos) = data_pos + 1, 0
            else:
                (data_pos, data_sub_pos) = data_pos + 1, 0

            if data_pos >= len(data_values):
                break
        return data_pos, data_sub_pos, param_values

    def __encode__(self, param_configs=None, param_values=None):
        enc_val = ''
        for name, config_info in param_configs.items():
            sub_param_val = param_values[name]
            (param_type, data_size, encoding) = config_info['type'], config_info['size'], config_info['encoding']
            if config_info['type'] == 'Struct':
                for val in sub_param_val:
                    enc_val += (self.__encode__(config_info['sub_parameters'], val))
                continue

            if encoding == 'plain':
                enc_val += str(sub_param_val)
            else:
                enc_val += self.encode_func[param_type](sub_param_val, data_size)
            if encoding != 'hex_stream':
                enc_val += ' '
        return enc_val

    def encode(self):
        enc_val = self.__encode__(self.parameters, self.parameter_values)
        if enc_val[-1] == ' ':
            enc_val = enc_val[:-1]
        return enc_val

    def decode(self, data_values):
        parameters = self.parameters
        data_pos, data_sub_pos, self.parameter_values = self.__decode__(data_values, parameters=parameters)

    def get(self, path=''):
        data_path_list = path.split('.')
        param_values = self.parameter_values
        for path in data_path_list:
            m = re.match(r'(\w+)\[(\d+)\]', path)
            param_name, value_index = path, 0
            if m is not None:
                param_name, value_index = m.group(1), int(m.group(2))

            if param_name in param_values:
                param_values = param_values[param_name]
                if type(param_values) == list and value_index < len(param_values):
                    param_values = param_values[value_index]
            else:
                return None
        return param_values

    def set(self, path, value):
        data_path_list = path.split('.')
        param_values = self.parameter_values
        param_configs = self.parameters

        if len(data_path_list) == 0:
            raise T32DataPacketError('[Data Set Error]Data Path is Null')

        last_param = data_path_list.pop()
        curr_path = ''
        for path in data_path_list:
            m = re.match(r'(\w+)\[(\d+)\]', path)
            param_name, value_index = path, 0
            if m is not None:
                param_name, value_index = m.group(1), int(m.group(2))

            curr_path = curr_path.join(param_name+'.')
            if param_name in param_values:
                param_values = param_values[param_name]
                param_configs = param_configs[param_name]

                if param_configs is None:
                    raise T32DataPacketError('[Data Set Error]Invalid path - ' + curr_path[:len(curr_path)-1])
                if param_configs['type'] == 'Struct':
                    for i in range(value_index - len(param_values) + 1):
                        param_values.append(self.__init_param_values__(param_configs['sub_parameters']))
                    param_values = param_values[value_index]
                    param_configs = param_configs['sub_parameters']
            else:
                return None

        type_mappings = {int:'Int', float:'Flaot', str:'String', bytes:'ByteArray'}
        if type(value) not in type_mappings:
            raise T32DataPacketError('[Data Set Error]Invalid type - ' + str(type(value)))
        if last_param not in param_values:
            raise T32DataPacketError('[Data Set Error]Param name mismatch : %s' % path)

        param_configs = param_configs[last_param]
        if type_mappings[type(value)] != param_configs['type']:
            raise T32DataPacketError('[Data Set Error]type mismatch - path:%s, type:val(%s),param(%s)'
                                     % (path, type_mappings[type(value)], param_configs['type']))
        param_values[last_param] = value


class T32DataParser:
    def __init__(self, data_path):
        self.data_struct_info = None
        self.message_list = None
        with open(data_path) as data_file:
            self.data_struct_info = json.load(data_file)
            self.parse()

    @staticmethod
    def parse_messages(messages):
        message_list = defaultdict(defaultdict)
        if messages is None or len(messages) == 0:
            return {}

        for key, value in messages.items():
            message_list[key] = T32DataParser.parse_parameters(value)
        return message_list

    @staticmethod
    def parse_parameters(parameters):
        param_list = defaultdict(defaultdict)
        if parameters is None or len(parameters) == 0:
            return {}

        for param_key, param_value in parameters.items():
            param_list[param_key] = defaultdict(defaultdict)
            param_list[param_key]['type'] = param_value.get('type', 'Int')
            param_list[param_key]['size'] = int(param_value.get('size', '0'))
            param_list[param_key]['encoding'] = param_value.get('encoding', 'hex')
            param_list[param_key]['array_size'] = int(param_value.get('array_size', '1'))
            param_list[param_key]['array_size_from'] = param_value.get('array_size_from', None)
            param_list[param_key]['sub_parameters'] = T32DataParser.parse_parameters(param_value.get('sub_parameters', None))
            param_list[param_key]['value'] = None

        return param_list

    def parse(self):
        if self.data_struct_info is None:
            return

        self.message_list = defaultdict(defaultdict)
        for msg_category in self.data_struct_info:
            self.message_list[msg_category] = T32DataParser.parse_messages(self.data_struct_info[msg_category])

    def create(self, name, interface_id=None):
        for key in self.message_list:
            if interface_id is None or interface_id == key:
                if name in self.message_list[key]:
                    return T32DataPacket(name, self.message_list[key][name])

        return None


if __name__ == "__main__":
    parser = T32DataParser(data_path="../message_format.json")
    data = parser.create("LELMBTAM_INIT_CNF")

    print('#'*50)
    print('>> Value Structure !!')
    print('-'*70)
    data.set('numOfDevice', 2)
    data.set('stLeKey.bd_addr', b'12345678')
    data.set('stLeKey[2].bd_addr', b'ABCDEFGH')
    pprint.pprint(data.parameter_values)
    print('#'*50)

    print('#'*50)
    data.decode(data_values=["2", "010203040506070800000122",
                             "010203640506070800000100666698j900000"])
    print(data.name)
    pprint.pprint(data.parameter_values)
    print('#'*50)

    device_cnt = data.get('numOfDevice')
    print('Num Of Device: %d' % device_cnt)
    for index in range(device_cnt):
        bd_addr = data.get('stLeKey[%d].bd_addr'%index)
        print('\t>#%02d BD_ADDR: %s' % (index, T32DataEncoder.enc_bytestr(bd_addr)))

    print('#'*50)
    enc_val = data.encode()
    print(enc_val)

    print('#'*50)
    val = T32DataEncoder.enc_string('2.443333', 4)
    val_decode = T32DataDecoder.decode_string(val)
    print('String :',val, val_decode)

    val = T32DataEncoder.enc_int(8844, size=4, to_bytestr=False)
    val_decode = T32DataDecoder.decode_int(val, size=4, encoding='bytes')
    print('Int(Size:4) :',val, val_decode)

    val = T32DataEncoder.enc_int(123456789012345, size=8, to_bytestr=True)
    val_decode = T32DataDecoder.decode_int(val, size=8, encoding='hex')
    print('Int(Size:8) :',val, val_decode)

    val = T32DataEncoder.enc_bytestr(b'\x00\x01\x02\xAA')
    val_decode = T32DataDecoder.decode_byte(val)
    print('ByteArray :', val, val_decode)

    val = T32DataEncoder.enc_bytestr(b'ABCD', fixed_size=7)
    val_decode = T32DataDecoder.decode_byte(val)
    print('ByteArray(FixLength) :', val, val_decode)
    print('###########################################')
