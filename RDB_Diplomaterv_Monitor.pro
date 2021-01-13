#===============================================================================#
#                                                                               #
#    RDB Diplomaterv Monitor                                                    #
#       A monitor program for the RDB Diplomaterv project                       #
#    Copyright (C) 2018  András Gergő Kocsis                                    #
#                                                                               #
#    This program is free software: you can redistribute it and/or modify       #
#    it under the terms of the GNU General Public License as published by       #
#    the Free Software Foundation, either version 3 of the License, or          #
#    (at your option) any later version.                                        #
#                                                                               #
#    This program is distributed in the hope that it will be useful,            #
#    but WITHOUT ANY WARRANTY; without even the implied warranty of             #
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              #
#    GNU General Public License for more details.                               #
#                                                                               #
#    You should have received a copy of the GNU General Public License          #
#    along with this program.  If not, see <https://www.gnu.org/licenses/>.     #
#                                                                               #
#===============================================================================#



TEMPLATE = subdirs

message(===============================)
message(=== RDB Diplomaterv Monitor ===)
message(===============================)
message(" ")
message(Qt version: $$[QT_VERSION])
message(Qt is installed in $$[QT_INSTALL_PREFIX])

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# The application will be always built
SUBDIRS += application

# The tests will be built only if explicitely requested
defined(BUILD_TESTS, var) {
    message(Tests will be built!)
    SUBDIRS += tests
} else {
    message("Tests will not be built, if you need them please define the \"BUILD_TESTS\" variable!")
    message("Example: \$ qmake BUILD_TESTS=\"On\" .")
}

DISTFILES += README.md
