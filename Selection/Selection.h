
//Select only one model!!
//#define IAQ_PROTOTYPE
//#define PHOENIX_PROTOTYPE
#define PHOENIX_PROTOTYPE2

#if (defined IAQ_PROTOTYPE && defined PHOENIX_PROTOTYPE && defined PHOENIX_PROTOTYPE2 )
	error if both selected
#endif




#if (defined IAQ_PROTOTYPE) // TO BE SURE IF ONLY ONE COMPILER SELECTED
	#define SI7013_ADDRESS 0x41  //Unshifted 7-bit I2C address if AD0 is floating or pulled high during bootup

	#define TSL2561_ADDRESS 0x39 //

	#define VOC_MEASURE_PIN  3   // Read Voc
	#define NOISE_MEASURE_PIN  1

#endif

#if (defined PHOENIX_PROTOTYPE) // TO BE SURE IF ONLY ONE COMPILER SELECTED
	#define SI7013_ADDRESS 0x40  //Unshifted 7-bit I2C address if AD0 is floating or pulled high during bootup		//cll,change 0x41 to 0x40 ,ad0 is low

	#define TSL2561_ADDRESS 0x29  						//cll,change 0x39 to 0x29


	#define VOC_MEASURE_PIN  4				//cll,change 3 to 4  Read Voc
	#define NOISE_MEASURE_PIN  5
#endif

#if (defined PHOENIX_PROTOTYPE2) // TO BE SURE IF ONLY ONE COMPILER SELECTED
	#define SI7013_ADDRESS 0x40  //Unshifted 7-bit I2C address if AD0 is floating or pulled high during bootup		//cll,change 0x41 to 0x40 ,ad0 is low

	#define TSL2561_ADDRESS 0x29  						//cll,change 0x39 to 0x29


	#define VOC_MEASURE_PIN  3				//cll,change 3 to 4  Read Voc
	#define NOISE_MEASURE_PIN  1
#endif


