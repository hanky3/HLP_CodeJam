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


class T32DataDecoder:
    @staticmethod
    def decode_byte(data, size, encoding='hex', endian='big'):
        end = (2 * size)
        if len(data) < end:
            raise T32DataPacketError("Decode Error : Length Mismatch : Data(%s), size(%d)" % (data, size))
        byte_val = bytes([int(data[pos:pos+2], 16) for pos in range(0, end, 2)])
        return byte_val

    @staticmethod
    def decode_int(data, size=2, encoding='hex', endian='big'):
        if encoding == 'number':
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
        if encoding == 'number':
            return float(data)

        if encoding == 'bytes':
            byte_value = data
        else:
            byte_value = T32DataDecoder.decode_byte(data, size)
            if endian == 'big':
                byte_value = list(reversed(byte_value))
        return struct.unpack('f', byte_value[:size])

    @staticmethod
    def decode_string(data, size, type='utf-8'):
        return T32DataDecoder.decode_byte(data, size).decode(type)


class T32DataPacket:
    def __init__(self, name, parameters):
        self.name = name
        self.parameters = parameters
        self.parameter_values = defaultdict()
        self.decode_func = None
        self.__init_decode_func__()

    def __init_decode_func__(self):
        self.decode_func = defaultdict()
        self.decode_func['ByteArray'] = T32DataDecoder.decode_byte
        self.decode_func['String'] = T32DataDecoder.decode_string
        self.decode_func['Int'] = T32DataDecoder.decode_int
        self.decode_func['Float'] = T32DataDecoder.decode_float
        self.decode_func['Struct'] = self.decode

    def __decode__(self, data_values, data_pos=0, data_sub_pos=0, parameters=defaultdict()):
        param_values = defaultdict()

        for name, meta_info in parameters.items():
            (param_type, data_size, encoding) = meta_info['type'], meta_info['size'], meta_info['encoding']
            if param_type not in self.decode_func:
                raise T32DataPacketError("[Decode Error]Invalid type(%s) - %s" % (type, name))
            if encoding not in ['number', 'hex', 'hex_stream']:
                raise T32DataPacketError("[Decode Error]Invalid encoding type(%s) - %s" % (encoding, name))

            if param_type == 'Struct':
                arr_size_from = meta_info['array_size_from']
                array_size = meta_info['array_size']
                if arr_size_from in param_values:
                    array_size = param_values[arr_size_from]

                param_values[name] = [defaultdict()]*array_size
                for pos in range(array_size):
                    (data_pos, data_sub_pos, param_values[name][pos]) \
                        = self.__decode__(data_values, data_pos=data_pos, data_sub_pos=data_sub_pos,parameters=meta_info['sub_parameters'])
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

    def decode(self, data_values, sub_param=None, sub_values=None, index=0, sub_index=0):
        parameters = self.parameters
        data_pos, data_sub_pos, self.parameter_values = self.__decode__(data_values, parameters=parameters)

    def encode(self):
        pass

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

    def set(self, path, data):
        pass


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
    parser = T32DataParser(data_path="..\message_format.json")
    data = parser.create("LELMBTAM_INIT_CNF")
    data.decode(data_values=["0002", "010203040506070800000120666698j90000", "010203640506070800000100666698j900000"])

    print(data.name)
    pprint.pprint(data.parameters)
    pprint.pprint(data.parameter_values)

    val = data.get('numOfDevice')
    val1, val2 = data.get('stLeKey[0].bd_addr'), data.get('stLeKey[1].bd_addr')
    print('BD_ADDR: ' + ''.join('%02X'%v for v in val1))
    print(''.join('%02X' % v for v in val2))
