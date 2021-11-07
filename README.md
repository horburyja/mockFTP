# Mock FTP client

FTP-like client for transfering files. The client provides 4 commands: ls, get, put, and exit.

## Project Details

The client performs ls, get, put or exit by constructing a Command, and sending it to the server, and then
sending or receiving a Payload struct, followed by text or the contents of a file. The CC_LS, CC_PUT, CC_GET,
and CC_EXIT are used in the code field of the Command struct, to indicate what command is being performed.

The Payload indicates whether it will be followed by text, or the contents of the file. The code field of the
Payload is either PL TXT or PL FILE, and the length field indicates how many bytes are being sent.

## LS Command

This command tells the server to send a list of files in the current working directory. The protocol works as follows:

1. The client send a Command to the server. The code field is set to CC_LS and the arg field is an array filled with zeros.
2. The server prepares a Payload, and sends it to the client.
3. The server prepares the list of files, and sends it to the client. Since this is just text, the code field of the Payload is set to PL_TXT, and the length field is set to the number of characters in the text.
4. The client prints the lsit of files