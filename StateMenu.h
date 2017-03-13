typedef struct {
	
	char* measurementString;
  char* unitString;
  char* rangeString;
	
  double scalingFactor;
  unsigned int typeMode; //MSB two bits of the control signal
  unsigned int rangeMode; //LSV two bits of the control signal
} state;

const state stateLookUp[10][10] = {
	{ //Voltage
    {//RANGE_10v
			"Voltage",
			"V",
			"10 V",
      20,
      0x2<<2,
      0x0
    },
		{//RANGE_1v
			"Voltage",
			"V",
			"1 V",
      2,
      0x2<<2,
      0x1
    },
		{//RANGE_100mv
			"Voltage",
			"mV",
			"100 mV",
      0.2,
      0x2<<2,
      0x2
    },
		{//RANGE_10mv
			"Voltage",
			"mV",
			"10 mV",
      0.02,
      0x2<<2,
      0x3
    },
  },
	
  { //Current
		{//RANGE_10A - never used.
			"10A Current",
			"",
			"",
      0,
      0x3,
      0x0
    },
		{//RANGE_1A
			"Current",
			"A",
			"1 A",
      1,
      0x1<<2,
      0x0
    },
		{//RANGE_100mA
			"Current",
			"A",
			"100 mA",
      0.1,
      0x1<<2,
      0x1
    },
		{//RANGE_1mA
			"Current",
			"A",
			"10 mA",
      0.01,
      0x1<<2,
      0x2
    },
  },
	
	{ //Resistance 
		{//RANGE 1Mohm
			"Resistance",
			"Mohm",
			"1 Mohm",
			1,
			0x0<<2,
			0x0
		},
		{//RANGE 100kohm
			"Resistance",
			"kohm",
			"100 kohm",
			100,
			0x0<<2,
			0x1
		},
		{//RANGE 10kohm
			"Resistance",
			"kohm",
			"10 kohm",
			10,
			0x0<<2,
			0x2
		},
		{//RANGE 1kohm
			"Resistance",
			"kohm",
			"1 kohm",
			1,
			0x0<<2,
		0x3
		}
	}
};

