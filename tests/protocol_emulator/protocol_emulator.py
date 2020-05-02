import logging
import sys
import os
import serial


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
            Start = R"<CMDP_H>\n"
            End = R">CMDP_H<\n"

        class Data:
            Start = R"<CMDP_D>\n"
            End = R">CMDP_D<\n"

        Tail = R"<CMDP_T>\n"

    def __init__(self, transmit_function):
        self._transmit_function = transmit_function

    def TransmitSession(self, diagram_title, data_line_titles, content_of_the_data_messages):
        self.TransmitHeader(diagram_title, data_line_titles)
        for data_message_content in content_of_the_data_messages:
            self.TransmitData(data_message_content)
        self.TransmitTail()

    def TransmitHeader(self, diagram_title, data_line_titles):
        self._transmit_function(CMDP_Emulator.MessageConstants.Header.Start)
        if diagram_title:
            self._transmit_function("<" + diagram_title + ">")
        header_content = "X:{},".format(data_line_titles[0])
        for index in range(1, len(data_line_titles)):
            header_content += "Y{}:{},".format((index - 1), data_line_titles[index])
        header_content += "\n"
        self._transmit_function(header_content)
        self._transmit_function(CMDP_Emulator.MessageConstants.Header.End)

    def TransmitData(self, data_points_of_the_message):
        self._transmit_function(CMDP_Emulator.MessageConstants.Data.Start)
        for data_points_of_the_measurement in data_points_of_the_message:
            data_content = "X:{},".format(data_points_of_the_measurement[0])
            for index in range(1, len(data_points_of_the_measurement)):
                data_content += "Y{}:{},".format((index - 1), data_points_of_the_measurement[index])
            data_content += "\n"
        self._transmit_function(CMDP_Emulator.MessageConstants.Data.End)

    def TransmitTail(self):
        self._transmit_function(CMDP_Emulator.MessageConstants.Tail)


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
        emulator = EmulatorFactory(protocol, serial_port.write)

        emulator.TransmitSession(None,
                                ("Time [s]", "Temperature [C]"),
                                (
                                    (
                                        {"X": "0", "Y0":"10"}
                                    ),
                                    (
                                        {"X":"10", "Y0":"12"},
                                        {"X":"15", "Y0":"13"}
                                    ),
                                    (
                                        {"X": "5", "Y0":"11"},
                                        {"X":"20", "Y0":"14"}
                                    )
                                ))

    except:
        logging.fatal("Could not open serial port")
    finally:
        if serial_port is not None:
            logging.info("Closing serial port")
            serial_port.close()


if "__main__" == __name__:
    # Setting the logging level
    logging.basicConfig(level=logging.DEBUG)
    # Running the emulator
    main()
