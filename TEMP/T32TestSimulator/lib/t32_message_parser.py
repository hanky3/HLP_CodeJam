from collections import *
import json
import pprint
import struct


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
    def decode_int(data, size, encoding='hex', endian='big'):
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
        self.decode_func = None
        self.init_decode_func()

    def init_decode_func(self):
        self.decode_func = defaultdict()
        self.decode_func['ByteArray'] = T32DataDecoder.decode_byte
        self.decode_func['String'] = T32DataDecoder.decode_string
        self.decode_func['Int'] = T32DataDecoder.decode_int
        self.decode_func['Float'] = T32DataDecoder.decode_float
        self.decode_func['Struct'] = self.decode

    def decode(self, data_values, sub_param=None, index=0, sub_index=0):
        parameters = self.parameters
        if sub_param is not None:
            parameters = sub_param

        for name, param_value in parameters.items():
            (param_type, data_size, encoding) = param_value['type'], param_value['size'], param_value['encoding']
            if param_type not in self.decode_func:
                raise T32DataPacketError("[Decode Error]Invalid type(%s) - %s" % (type, name))
            if encoding not in ['number', 'hex', 'hex_stream']:
                raise T32DataPacketError("[Decode Error]Invalid encoding type(%s) - %s" % (encoding, name))

            if param_type == 'Struct':
                (index, sub_index) = self.decode(data_values, sub_param=param_value['sub_parameters'], index=index, sub_index=sub_index)
                continue

            param_value['value'] = self.decode_func[param_type](data_values[index][sub_index:], data_size, encoding=encoding)
            if encoding == 'hex_stream':
                sub_index += (data_size*2)
                if sub_index >= len(data_values[index]):
                    (index, sub_index) = index + 1, 0
            else:
                (index, sub_index) = index + 1, 0

            if index >= len(data_values):
                break
        return index, sub_index

    def encode(self):
        pass

    def get(self, path, pos=0, size=0, data_type=int):
        pass

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
            param_list[param_key]['array_size'] = int(param_value.get('array_size', '0'))
            param_list[param_key]['array_size_from'] = param_value.get('array_size_from', '')
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
    data.decode(data_values=["0001", "010203040506070800000100"])

    print(data.name)
    pprint.pprint(data.parameters)

    print(T32DataDecoder.decode_int("0013", 2))
    bd_addr = data.parameters['stLeKey']['sub_parameters']['bd_addr']['value']
    print(''.join("%02X"%x for x in bd_addr))
    print(T32DataDecoder.decode_int(bd_addr, size=2, encoding='bytes'))
