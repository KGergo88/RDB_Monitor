import logging
import sys
import os
import serial
import collections

import emulators.emulator_factory
import emulators.cmdp


# socat command to create the serial port pair:
# socat -d -d pty,raw,echo=0 pty,raw,echo=0

class Constants:
    # Set this variable to a device (like /dev/pts/3) or
    # to None if the user should enter the name during runtime
    DEFAULT_DEVICE = "/dev/pts/3"
    # Set this variable to a name of one of the available protocols or
    # to None if the user should enter the name during runtime
    DEFAULT_PROTOCOL = "MDP"


def getDevice(default_device):
    device = default_device

    while device is None:
        device = input("Please enter the name of the device on which the data needs to be sent!\ndevice: ")
        if not os.path.exists(device):
            logging.fatal("The entered device \"{}\" does not exist!".format(device))
            device = None

    return device


def getProtocol(default_protocol, available_protocols):
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


def testWithSerialConnection(protocol):
    logging.info("Testing the emulator with serial connection")
    device = getDevice(Constants.DEFAULT_DEVICE)
    logging.info("Selected device: {}".format(device))

    serial_port = None
    logging.info("Opening serial port")
    try:
        serial_port = serial.Serial(device)
        logging.info("Creating emulator")
        emulator = emulators.emulator_factory.createEmulator(protocol, serial_port.write, "ascii")

        logging.info("Transmitting session")
        emulator.TransmitSession("TestDiagram",
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


def testOffline(protocol):
    logging.info("Testing the emulator offline")

    emulator = emulators.emulator_factory.createEmulator(protocol, print, "ascii")

    logging.info("Transmitting session")
    emulator.transmitSession("TestDiagram",
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


if "__main__" == __name__:
    # Setting the logging level
    logging.basicConfig(level=logging.DEBUG)

    protocol = getProtocol(Constants.DEFAULT_PROTOCOL,
                           emulators.emulator_factory.AVAILABLE_PROTOCOLS.keys())
    logging.info("Selected protocol: {}".format(protocol))

    # Running the offline emulator test
    testOffline(protocol)
