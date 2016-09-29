	PRESERVE8

	AREA HVectors, CODE, READONLY
	
	EXPORT HIGH_VECTORS

	IMPORT ResetHandler
	IMPORT UndefHandler
	IMPORT SWIHandler
	IMPORT PrefHandler
	IMPORT DataHandler
	IMPORT IrqHandler
	IMPORT FiqHandler

HIGH_VECTORS
	B ResetHandler
	B UndefHandler
	B SWIHandler
	B PrefHandler
	B DataHandler
	NOP ; Reserved vector
	B IrqHandler
	B FiqHandler

	END
