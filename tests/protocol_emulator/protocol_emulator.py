import logging
import sys
import os
import serial
import collections


# socat command to create the serial port pair:
# socat -d -d pty,raw,echo=0 pty,raw,echo=0

class Constants:
    # Set this variable to a device (like /dev/pts/3) or to None if the used should enter the name during runtime
    DEFAULT_DEVICE = "/dev/pts/3"
    # The protocols that are supported by the emulator
    AVAILABLE_PROTOCOLS = ("MDP", "CMDP")
    # Set this variable to one of the AVAILABLE_PROTOCOLS or to None if the used should enter the name during runtime
    DEFAULT_PROTOCOL = AVAILABLE_PROTOCOLS[1]


class CMDP_Emulator:
    class MessageConstants:
        class Header:
            Start = R"<CMDP_H>"
            End = R">CMDP_H<"

        class Data:
            Start = R"<CMDP_D>"
            End = R">CMDP_D<"

        Tail = R"<CMDP_T>"

    def __init__(self, transmit_function, string_encoding):
        self._transmit_function = transmit_function
        self._string_encoding = string_encoding

    def _transmit_line(self, string_to_transfer):
        self._transmit_function(bytes(string_to_transfer, self._string_encoding) + b"\r\n")

    def TransmitSession(self, diagram_title, data_line_titles, content_of_the_data_messages):
        """
        diagram_title : str
            The title of the diagram. Set it to None there is none.
        data_line_titles : dict/OrderedDict
            The titles of the data lines. The keys of the dict/OrderedDict must be X, Y0 ... Yn.
            If the order of the data line titles is important for your use case use an OrderedDict,
            otherwise you can use a regular dict.
            Example:
                {"X":"Time [s]", "Y0":"Temperature [C]"}
        content_of_the_data_messages : list/tuple of list/tuples of dicts/OrderedDicts
            Contains the data for all the data messages of the session.
            The dicts/OrderedDicts represent one measurement point with one or more values.
            The keys of the dict/OrderedDict must be X, Y0 ... Yn.
            If the order of the elements in a measurement point is important for your use case use an OrderedDict,
            otherwise you can use a regular dict.
            One list/tuple element represent one data message. Example:
            (
                (
                    {"X": "0", "Y0":"10"},
                ),
                (
                    {"X":"10", "Y0":"12"},
                    {"X":"15", "Y0":"13"}
                ),
                (
                    {"X": "5", "Y0":"11"},
                    {"X":"20", "Y0":"14"}
                )
            )
        """
        self.TransmitHeader(diagram_title, data_line_titles)
        for data_message_content in content_of_the_data_messages:
            self.TransmitData(data_message_content)
        self.TransmitTail()

    def TransmitHeader(self, diagram_title, data_line_titles):
        self._transmit_line(CMDP_Emulator.MessageConstants.Header.Start)
        if diagram_title:
            self._transmit_line("<" + diagram_title + ">")
        line_to_send = ""
        for key, value in data_line_titles.items():
            line_to_send += "{}:{},".format(key, value)
        self._transmit_line(line_to_send)
        self._transmit_line(CMDP_Emulator.MessageConstants.Header.End)

    def TransmitData(self, data_message_content):
        self._transmit_line(CMDP_Emulator.MessageConstants.Data.Start)
        for measurement_point_data in data_message_content:
            line_to_send = ""
            for key, value in measurement_point_data.items():
                line_to_send += "{}:{},".format(key, value)
            self._transmit_line(line_to_send)
        self._transmit_line(CMDP_Emulator.MessageConstants.Data.End)

    def TransmitTail(self):
        self._transmit_line(CMDP_Emulator.MessageConstants.Tail)


def GetDevice(default_device):
    device = default_device
    while device is None:
        device = input("Please enter the name of the device on which the data needs to be sent!\ndevice: ")
        if not os.path.exists(device):
            logging.fatal("The entered device \"{}\" does not exist!".format(device))
            device = None
    return device

def GetProtocol(default_protocol, available_protocols):
    protocol = default_protocol
    while protocol is None:
        print("Please enter the number of the protocol with which the data needs to be sent!")
        print("Available protocols:")
        for element in available_protocols:
            print("{}-->{}".format(available_protocols.index(element), element))
        protocol_index = input("Selected protocol: ")
        try:
            protocol = available_protocols[int(protocol_index)]
        except:
            print("Invalid protocol...")
            protocol = None
    return protocol

def EmulatorFactory(protocol, *args):
    emulator = None
    if "CMDP" == protocol:
        emulator = CMDP_Emulator(*args)
    else:
        logging.fatal("Unexpected protocol value: {}".format(protocol))
        raise ValueError()
    return emulator


def main():
    device = GetDevice(Constants.DEFAULT_DEVICE)
    logging.info("Selected device: {}".format(device))

    protocol = GetProtocol(Constants.DEFAULT_PROTOCOL, Constants.AVAILABLE_PROTOCOLS)
    logging.info("Selected protocol: {}".format(protocol))

    serial_port = None
    try:
        logging.info("Opening serial port")
        serial_port = serial.Serial(device)

        logging.info("Creating emulator")
        emulator = EmulatorFactory(protocol, serial_port.write, "ascii")

        logging.info("Transmitting session")
        emulator.TransmitSession(None,
                                 collections.OrderedDict([("X", "Time [s]"), ("Y0", "Temperature [C]")]),
                                 (
                                     (
                                         collections.OrderedDict([("X", "0"), ("Y0", "10")]),
                                     ),
                                     (
                                         collections.OrderedDict([("X", "10"), ("Y0", "12")]),
                                         collections.OrderedDict([("X", "15"), ("Y0", "13")])
                                     ),
                                     (
                                         collections.OrderedDict([("X",  "5"), ("Y0", "11")]),
                                         collections.OrderedDict([("X", "20"), ("Y0", "14")])
                                     )
                                 ))
    finally:
        if serial_port is not None:
            logging.info("Closing serial port")
            serial_port.close()


if "__main__" == __name__:
    # Setting the logging level
    logging.basicConfig(level=logging.DEBUG)
    # Running the emulator
    main()
