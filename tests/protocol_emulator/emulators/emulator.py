import abc


class Emulator(abc.ABC):
    def __init__(self, transmit_function, string_encoding):
        """
        Constructor.
        transmit_fuction : callable
            The function that shall be used to transmit the data.
            This will be called with an array of bytes.
            It can be for example the write() member of the serial.Serial class,
            or the print() for testing the emulator classes.
        string_encoding : str
            The encoding that shall be used for the strings that will be transmitted.
            For possible valies see the bytes() built-in function.
        """
        self._transmit_function = transmit_function
        self._string_encoding = string_encoding

    @abc.abstractmethod
    def transmitSession(self, diagram_title, data_line_titles, content_of_the_data_messages):
        """
        Function to transmit a whole measurement session.
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
        pass

    def _transmit_line(self, string_to_transfer, line_ending="\r\n"):
        """
        Function to transmit a single line.
        string_to_transfer : str
            The content of the line that needs to be transmitted.
        """
        self._transmit_function(bytes(string_to_transfer + line_ending, self._string_encoding))
