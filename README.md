# NodesGraph
A simple implementation of nodes and their connections.

NOTICE: THIS PROGRAM IS BEING PORTED TO ANOTHER PROGRAM OF MINE. IT WILL REMAIN ON GITHUB.COM, BUT NO FURTHER CHANGES WILL BE MADE TO THIS REPO.
ANY ISSUES WILL BE CONSIDERED AND FIXED IN THE FINAL PRODUCT OF MY OTHER PROGRAM.

It uses a keyboard based interface for interacting with the program.

# Selecting Nodes & Connections(Links)
1. There are three ways to select nodes and connections:
   1. Press 1-9 to select their corresponding node.
   2. Press 'N' or 'n' to tell the program you are typing a number that is more than one chararacter, and then type the number you want. You can use backspace for this. When finished, press the RETURN or ENTER key to process the number.
   3. Select the target node with the mouse pointer/touch. (NODES ONLY, NOT CONNECTIONS)
   PS: If the number you typed in is not currently exisiting, the program will give you an error.
2. With nodes selected you can preform the other operations in the program.
3. Selected nodes/connections will show up with a white border on the screen.
4. With a Connection/Link or a Node connected, you can press the DELETE key to remove it. (NOTE: Deleting a node will remove all of it's connections as well.)

# Modes & How they are used
There are a few modes in this program, and each are used to create and edit data in the program.

1. Selection Mode: This mode is the standard mode, and it allows you to simply select a node. This mode is active when no other mode is active.
2. Move Mode: This mode will allow you to move a node around by selecting it, and then clicking on a place in the viewport, and it will move the node to that point. To Access this mode, press M. When in Move mode, if you press an hold shift while a node is selected, it will follow the cursor as long is shift is held down.
3. Connection Selection Mode: Similar to Selection Mode but only deals with connections. To Access this mode, press C.
4. Add Node Mode: Adds nodes to the viewport. By pressing down on the viewport, a node will be created in the spot you placed. To Access this mode, press A.
5. Link Mode: This mode is for adding a Connection/Link to two different nodes. To begin, press L to activate this mode. Then, select your first node. Then select your second node. If a connection already exists between the two nodes, an error will pop up. If not, the connection will be created. NOTE: A node cannot be linked to itself.
6. Accepting Number Mode: Used for selecting a node. Refer to the Selecting Nodes & Connections section for more information. To toggle this mode, press N.
7. Radius Changing Mode: Press 'R' to enter this mode, which allows you to adjust the smoothness of the connection lines. By pressing 0-9, or using number selection (See Selecting Nodes & Connections for more instructions), it will change the radius of the viewport in realtime.

# Basic User Interface
1. To drag around in the viewport, press and hold the left mouse button. The green square in the middle of the screen is the origin.
2. Press 'O' to center the origin with the screen.
3. Press Escape to clear all modes and go to Selection Mode.
4. Press Escape and Shift at the same time to completeley clear out the viewport.
5. In the top left corner there is a text output that will show the current state of the program and what mode you are in.
6. At any time, press 'S' to clear all selections without clearing the mode.

# Things to add in the future:
1. Custom Colors for connections & nodes. (In Progress, Colors will need to be updated, but the connections are on the way)
2. Custom text for nodes.
3. Selection by mouse pointer. (DONE)
4. Files, Loading and Saving of Schemas. (In Progress)
