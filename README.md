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

## GET Command

This is when the client requests a file from the server. It works as follows:

1. Client prepares a Command, and sends it to the server. The code field is sent to CC_GET, and
the arg field is set to the name of the file, including the null terminator.
2. The server prepares a Payload and sends it to the client. The code field is set to (network
byte ordered) PL_FILE, and length is set to the size of the file in bytes (again, in network byte
order).
3. The server calls the sendFileOverSocket function to send the file contents to the client.
4. The client receives the file, and saves it to disk, appending “.download” to the name of the file.

## PUT Command

This function does the inverse of GET: it sends a file to the server. It works as follows:

1. Client constructs a Command, and sends it to the server. The code is set to the (network byte ordered)
CC_PUT, while the name of the file is placed in the arg field.
2. The client computes the size of the file using the getFileSize function. It prepares
a Payload, where the code is (network byte ordered) PL_FILE, and the length field is the size of the
file. The client sends the payload to the server.
3. The client calls the sendFileOverSocket function to send the file contents to the server.
