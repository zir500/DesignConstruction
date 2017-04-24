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
      0x2,
      0x0
    },
		{//RANGE_1v
			"Voltage",
			"V",
			"1 V",
      2,
      0x2,
      0x1
    },
		{//RANGE_100mv
			"Voltage",
			"mV",
			"100 mV",
      0.2*1000, //1000 facter to convert to milivolts
      0x2,
      0x2
    },
		{//RANGE_10mv
			"Voltage",
			"mV",
			"10 mV",
      0.02*1000,
      0x2,
      0x3
    },
		{//RANGE_1mv
			"Voltage",
			"mV",
			"1 mV",
      0.002*1000,
      0x2,
      0x4
    },
  },
	
  { //Current
		{//RANGE_10A - never used.
			"Error! Out of Current range",
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
      2,
      0x1,
      0x0
    },
		{//RANGE_100mA
			"Current",
			"mA",
			"100 mA",
      0.2*1000,
      0x1,
      0x1
    },
		{//RANGE_1mA
			"Current",
			"mA",
			"10 mA",
      0.02*1000,
      0x1,
      0x2
    },
  },
	
	{ //Resistance 
		{//RANGE 1Mohm
			"Resist",
			"Mohm",
			"1 Mohm",
			1,
			0x0,
			0x0
		},
		{//RANGE 100kohm
			"Resis",
			"kohm",
			"100 kohm",
			100,
			0x0,
			0x1
		},
		{//RANGE 10kohm
			"Resist",
			"kohm",
			"10 kohm",
			10,
			0x0,
			0x2
		},
		{//RANGE 1kohm
			"Resist",
			"kohm",
			"1 kohm",
			1,
			0x0,
			0x3
		}, 
		{//RANGE continuity
			"Continuity",
			"ohms",
			"",
			450,
			0x0,
			0x4
		}
	}
};

