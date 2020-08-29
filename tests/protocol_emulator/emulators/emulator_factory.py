import logging

import emulators.cmdp


# The protocols that are supported
AVAILABLE_PROTOCOLS = {
    "MDP" : None,
    "CMDP" : emulators.cmdp.CMDP_Emulator,
}


def createEmulator(protocol, *args):
    emulator = None
    
    if protocol in AVAILABLE_PROTOCOLS:
        emulator = AVAILABLE_PROTOCOLS[protocol](*args)
    else:
        logging.fatal("Unexpected protocol value: {}".format(protocol))
        raise ValueError()

    return emulator
