Electrocardio
=============
* Add (Boolean) material property that shows the de/repolarization status of the cells
* Make stimulation parameters adjustable from outside (e.g.. via user-defined functions)

Cardiac Mechanics
=================
* Couple to displacements through a coupled vector instead of individual disp_.. variables.
	* for details see
	* A working example is found in CardiacMechanicsMaterial.C
	* All affected files can be found via '''grepc -iR \"disp src/*'''

