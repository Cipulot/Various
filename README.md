# Scripts
A collection of scripts written in various programming languages. You can find all sorts of stuff in here.

FUNCTIONALITY
-wfn_generator_email.py:
This script is designed to extract AIMPAC data from an output file (.dat) of the GAMESS program. If valid data is found these are written to a .wfn file (wavefunction file). After the newly generated file have been saved the script will automatically send it to a designated email address specified into the script. NOTE: the script will check for the dimension of the file to ensure that can be attached to the email. The size limit is of 25MB for Gmail.
