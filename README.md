# NodesGraph
A simple implementation of nodes and their connections.


It uses a keyboard based interface for interacting with the program.

# Modes
In the top left corner, it shows the current mode. There are the following modes:
1. Selection Mode: The mode by default, in which moving around the viewport is done by pressing and holding the left mouse button. By pressing on any key 1-9, the program will select the following box. Currently, only 9 boxes are allowed.
2. Add New Node Mode: The mode created by pressing and holding SHIFT, it allows the user to create a new node by pressing on the screen where they would like the new node. It will be assigned a number, which can be used to select it later in selection mode. As soon as the user releases the SHIFT key, Add New Mode will be toggled off.
3. Move Mode: The mode in which nodes can be moved arround. First, press M to toggle this mode, then select the node you want to move by pressing 1-9. Match the number on the node to move with that of the keyboard button pressed. Then, left click anywhere in the viewport to move the node to that new location.
4. Link Mode: The mode that nodes can be connnected with. To start, press L to go into link mode. Then, select a node by matching the node's number to that of the keyboard button. Then, select a secondary node by matching its number to the keyboard button. Then, a link will connect the two nodes together.

To exit a toggle mode (Like Move or Link), press their activation key to toggle them off, or press ESC to go back to Selection Mode.

# Basic User Interface
1. Press and Hold the left mouse button down to drag the viewport arround. The green square in the viewport is the origin, and that is (0, 0) for all of the nodes.
2. Press and hold control and move the mouse around to do somooth zooming. Scroll the scroll wheel to do standard zooming.
3. Press 'O' to center the origin & viewport to the center of the screen and reset zooming.
4. Press the numbers 1-9 to select a node. See more on this in the Modes Section.
5. The status of the program is displayed in the top left corner.
