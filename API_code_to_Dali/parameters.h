/*****************************************************************************************

    This files defines all the global variables and parameters

******************************************************************************************/

#define DYNAMIC_BOUNBDS



	
#define use_RM					0

#define NumOnRamp           16	   // the number of onramp; SR99
#define SecSize             17
	
#define Occ_thresh			20.0        // 04_20_10; 25: 05_12_12; 12.5 used for I-80 which was good; 15.0 used
#define rm_band				400.0
#define reduction_coeff		0.075
#define increase_coeff		0.15
#define RELEASE_CYCLE_LEN   10.0
#define RELEASE_OCC         70.0
#define DEMAND_CHANGE       0           // 0: no change; 1: selective onramp dmd change; 2: all onramp dmd change // 02_20_14


#define ALLvehType          0
#define ShortSecSize 30
#define NumLnDetector       27     //
#define NumCrossDetector    30
#define max_onramp_ln       3
#define max_mainline_ln		5
#define dmd_up_t            (30.0*60.0)     //[s]: 6:30am; this needs to be changed dynamically
#define dmd_dwn_t           (120.0*60.0)    //[s]: 8:00am
#define dmd_change			(0.05)			// demand of each onramp incresaed by 5%
#define dmd_slice_len       300             // 5[min]  or 300[s] per slice
#define max_RM_rt			950             // 1080.0  // Field used value
#define N_interv			15              // number of RM rate based occ threshold intervals

#define Np 3                       // default: 3 same results as 5

static int ISUPDATE2=1;

#define	exp_flt 0.85           // sensor measure exp filter gain

#define len_str 1280           // for using sprintf_s
bool ControlOn;
int StateOn=0;
int StateOff=0;

double u[SecSize]={0};    // For all cells
static double q[SecSize]={0.0};   // composite mainline flow
static double v[SecSize]={0.0};   // composite speed for each cell
static double o[SecSize]={0.0};   // composite occupancy for each cell
double qc[SecSize]={7200};      // mainline capacity; assigned in Init()
double queue[SecSize]={0.0};      // updated with update_queue();
double s[NumOnRamp]={0.0};          // off-ramp flow
double q_main[SecSize]={0.0};     // mainline flow of all cells   
static double R[NumOnRamp]={0.0};   // composite onramp flow;  changed on 01/03/13
static float dmd[NumOnRamp]={0.0};          // onramp demand flow;     changed on 01/03/13


static double metering_rate_change_time=0.0;

unsigned int replication=0;
static int detectorNum=0,sectionNum=0,nodeNum=0,centroidNum=0;  // moved from data_save.h and readdetectordata.h; 05_29_13

static double T=1/60.0/2;  // 30s RM control time interval


static double dyna_min_r[NumOnRamp]={0.0};
static double dyna_max_r[NumOnRamp]={0.0};
static double Ramp_rt[NumOnRamp]={0.0,0.0,0.0};
static double RM_occ[NumOnRamp]={0.0,0.0,0.0};
static double ln_metering_rate[NumOnRamp][max_onramp_ln];
const int ln_meteringId[NumOnRamp][max_onramp_ln]={{21044,21045,21046},{19780,0,0},{19782,0,0},{19789,0,0},{21063,21062,21060},
                       {19892,0,19796},{21080,21081,21082},{19924,0,19801},{19936,0,19804},{19811,0,0},{19968,0,19815},
                       {19959,0,19819},{19948,0,19823},{19827,0,0},{19831,0,0},{19836,0,0}};



	static int release_cycle[NumOnRamp][max_onramp_ln]={{0,0,0}};	
	static double actual_r[NumOnRamp][max_onramp_ln]={{0.0,0.0,0.0}};
	
	const double L[SecSize]={1012.9, 895.8, 1333.8,498.2,1195.2,555.2,1562.0,455.4,1971.8,440.2,2402.1,427.6,1509.9,374.4,1374.9,377.9,1863.9};  // composite length; 1st section has no meter; 10_21_13
	const double Q[SecSize]={2000.0,4000.0,2000.0,2000.0,2000.0,2050.0,2000.0,2050.0,2050.0,2050.0,2000.0,2050.0,2050.0,2050.0,2000.0,2000.0,2000.0};	 //onramp flow capacity
	const double lambda[SecSize]={3.0000,3.0000,3.0000,3.0000,3.0000,3.0000,4.0000,3.3654,4.0000,4.0000, 4.0000,4.0000,4.2945,4.0000,4.1191,5.0000,4.6147}; // composite ln number	
	const int CellDetectionId[SecSize]={19775,19776,19779,19781,19788,19791,19795,19798,19800,19803,19810,19814,19818,19822,19826,19830,19834};  // 10_24_13; most downstream not detected
	
	const int N_Mainline_Ln_RM[NumOnRamp]={3,3,3,3,3,3,4,3,4,4,4,4,4,4,4,5};
	const int N_OnRamp_Ln[NumOnRamp]={3,1,1,2,3,2,3,2,2,1,2,2,2,1,1,1};
	const int OnRamp_Ln_Id[NumOnRamp][max_onramp_ln]={{19852,19842,17550},{338,0,0},{336,0,0},{19978,0,2218},{19875,19871,17232},{19884,0,17256},{19904,19896,17264},
                         {19916,0,17324},{19928,0,17336},{17392,0,0},{19964,0,17510},{19955,0,16705},{19940,0,16731},{16785,0,0},{16571,0,0},{16853,0,0}};

	const int HOV_Ln_Id[NumOnRamp]={3,0,0,0,3,3,3,3,3,0,3,3,3,0,0,0};
	const int Mainline_Ln_Det_Id[NumOnRamp][max_mainline_ln]={{18392,18394,0,0,18390},{18378,18380,0,0,18376},{18371,18373,0,0,18369},{18357,18359,0,0,18355},
				{18667,18669,0,0,18665},{18336,18338,0,0,18334},{18327,18329,18331,0,18325},{18311,18309,0,0,18313},
				{18302,18304,18306,0,18300},{18275,18277,18279,0,18273},{18266,18268,18270,0,18264},{18512,18514,18516,0,18510},
				{18228,18230,18232,0,18226},{18192,18194,18196,0,18190},{18201,18203,18205,0,18199},{18535,18537,18539,18541,18533}};

//for measuremnt only
const int onramp_secId[NumOnRamp]={17550,338,336,2218,17232,17256,17677,17324,17336,17392,17510,16705,16731,16785,16571,16853};   // 16 onramps; not used anymore; divided to lanes
const double onrampL[NumOnRamp]={320.7,347.5,705.3,397.6,652.666,379.2,453.6,587.6,831.7,284.8,615.9,334.6,470.7,402.7,522.7,272.3};
const int OnRampDetectionEndId[NumOnRamp]={21053,19784,19783,19790,19808,19797,20139,19802,19806,19812,19816,20140,19824,19828,19832,19835}; // single detector after meter

const int OnRampLnDetAfterMeterId[NumOnRamp][max_onramp_ln]={{21047,21048,21049},{91784,0,0},{19783,0,0},{19790,0,0},{21057,21058,21059},
                       {21071,0,2107},{21074,21075,21076},{21083,0,21084},{21087,0,21088},{19812,0,0},{21092,0,21093},
                       {19961,0,19820},{21098,0,21099},{19828,0,0},{19832,0,0},{19835,0,0}};

const int OnRamp_Ln_AdvanceDetEndId[NumOnRamp][max_onramp_ln]={{19860,19850,18679},{18418,0,0},{18419,0,0},{19980,0,0},
                       {21067,21068,21069},{19886,0,18424},{19906,19898,18425},{19922,0,18689},{19934,0,18691},{18434,0,0},
                       {19966,0,18436},{19957,0,18438},{19944,0,18693},{18442,0,0},{18444,0,0},{18446,0,0}};

bool Green[NumOnRamp][max_onramp_ln]={{false,false,false},{false,false,false},{false,false,false},{false,false,false},
                                      {false,false,false},{false,false,false},{false,false,false},{false,false,false},
									  {false,false,false},{false,false,false},{false,false,false},{false,false,false},
									  {false,false,false},{false,false,false},{false,false,false},{false,false,false}};

const double SR99_RM_occ_tbl[N_interv][NumOnRamp]={{   5,       6,       6,       6,    6,      4,     6,     5,     5,    6,    7,    8,     6,     9,     7,       5},
    { 5.9,     6.3,     6.3,     6.3,  6.9,    4.8,   6.9,   6.1,   6.1,  6.3,  7.4,  8.7,   6.9,  10.5,   7.7,     5.9},
    { 6.9,     6.5,     6.5,     6.5,  7.7,    5.6,   7.7,   7.1,   7.1,  6.5,  7.7,  9.4,   7.7,    12,   8.5,     6.9},
    { 7.8,     6.8,     6.8,     6.8,  8.6,    6.4,   8.6,   8.2,   8.2,  6.8,  8.1, 10.1,   8.6,  13.5,   9.3,     7.8},
    { 8.8,       7,       7,       7,  9.4,    7.1,   9.4,   9.2,   9.3,    7,  8.4, 10.9,   9.4,    15,  10.1,     8.8},
    { 9.7,     7.3,     7.3,     7.3, 10.3,    7.9,  10.3,  10.3,  10.4,  7.3,  8.8, 11.6,  10.3,  16.5,  10.9,     9.7},
    {10.7,     7.5,     7.5,     7.5, 11.1,    8.7,  11.1,  11.3,  11.4,  7.5,  9.1, 12.3,  11.1,    18,  11.7,    10.7},
    {11.6,     7.8,     7.8,     7.8,   12,    9.5,    12,  12.4,  12.5,  7.8,  9.5,   13,    12,  19.5,  12.5,    11.6},
    {12.5,       8,       8,       8, 12.9,   10.3,  12.9,  13.5,  13.6,    8,  9.9, 13.7,  12.9,    21,  13.2,    12.5},
    {13.5,     8.3,     8.3,     8.3, 13.7,   11.1,  13.7,  14.5,  14.6,  8.3, 10.2, 14.4,  13.7,  22.5,    14,    13.5},
    {14.4,     8.5,     8.5,     8.5, 14.6,   11.9,  14.6,  15.6,  15.7,  8.5, 10.6, 15.1,  14.6,    24,  14.8,    14.4},
    {15.4,     8.8,     8.8,     8.8, 15.4,   12.6,  15.4,  16.6,  16.8,  8.8, 10.9, 15.9,  15.4,  25.5,  15.6,    15.4},
    {16.3,       9,       9,       9, 16.3,   13.4,  16.3,  17.7,  17.9,    9, 11.3, 16.6,  16.3,    27,  16.4,    16.3},
    {17.3,     9.3,     9.3,     9.3, 17.1,   14.2,  17.1,  18.7,  18.9,  9.3, 11.6, 17.3,  17.1,  28.5,  17.2,    17.3},
    {18.2,     9.5,     9.5,     9.5,   18,     15,    18,  19.8,    20,  9.5,   12,   18,    18,    30,    18,    18.2}};
const double SR99_RM_rate_tbl[N_interv][NumOnRamp]={{1080,    1000,    1000,    1000,     750,     1000,    750,   910,   900,  1000,   990,  900,  900,    750,   920,    1080},
    {1060,     966,     966,     966,     740,      963,    740,   894,   853,   966,   970,  870,  870,    730,   895,    1060},
    {1040,     932,     932,     932,     729,      926,    729,   878,   806,   932,   949,  840,  840,    711,   870,    1040},
    {1020,     898,     898,     898,     718,      889,    718,   861,   759,   898,   928,  810,  810,    692,   846,    1020},
    {1000,     836,     836,     836,     708,      852,    708,   845,   712,   836,   908,  780,  780,    672,   821,    1000},
    { 980,     829,     829,     829,     696,      815,    696,   828,   665,   829,   887,  750,  750,    653,   796,     980},
    { 960,     795,     795,     795,     686,      778,    686,   812,   618,   795,   866,  720,  720,    634,   772,     960},
    { 940,     760,     760,     760,     675,      740,    675,   796,   570,   760,   845,  690,  690,    615,   747,     940},
    { 920,     726,     726,     726,     665,      703,    665,   779,   523,   726,   825,  660,  660,    595,   722,     920},
    { 900,     692,     692,     692,     654,      666,    654,   763,   476,   692,   804,  630,  630,    576,   698,     900},
    { 880,     658,     658,     658,     643,      629,    643,   746,   429,   658,   783,  600,  600,    557,   673,     880},
    { 860,     623,     623,     623,     633,      592,    633,   730,   382,   623,   763,  570,  570,    537,   648,     860},
    { 840,     589,     589,     589,     622,      555,    622,   713,   335,   589,   742,  540,  540,    518,   624,     840},
    { 820,     555,     555,     555,     611,      518,    611,   697,   288,   555,   721,  510,  510,    499,   599,     820},
    { 800,     520,     520,     520,     600,      480,    600,   681,   240,   520,   700,  480,  480,    480,   575,     800}};

const int OffRampDetectionId[(NumOnRamp)]={0, 18417,  0, 19787,0, 18423, 0, 18426, 0, 18433, 18435, 18437, 18439,18441,18443,18445}; // OffRamp Matched with OnRamp
const int offramp_secId[NumOnRamp]=     {0,  337,   0, 2217, 0, 17246, 0, 17286, 0, 17384, 17500, 16697, 16727,16751,16565,16833}; // OffRamp Matched with OnRamp



int origin[20]={0};
int destination[20]={0};


bool InitRealtimeDetection(void);
bool InitRealtimeDetection_s(void);	// memory allocation; for control detection, almost the same as InitRealtimeDetection(), just has less detector and not save data
bool InitRealTimeSection(void);	//for section measure

// from data_save.h
int open_detector(char* data_saving, unsigned int replic);
int open_section(char* data_saving, unsigned int replic);
int open_system(char* data_saving, unsigned int replic);
int open_detector_instant(char* data_saving, unsigned int replic);
int open_section_instant(char* data_saving, unsigned int replic);
int open_network(char* data_saving, unsigned int replic);
int open_signal(char* data_saving, unsigned int replic);
int init_data_saving(unsigned int replica);
int get_detIds(void);
int get_sectIds(void);
int get_nodeIds(void);
int read_detector(double);
int read_detector_instant(double);
int read_section(double);
int read_system(double);
int read_section_instant(double);
int read_meter_state(double);
int read_signal_state(double);
int save_networkinfo(char * data_saving, unsigned replic);


double detInterval,detInstantInterval,last_det_readtime,last_sect_readtime,sectStatInterval,last_syst_readtime,systStatInterval,last_det_inst_readtime;
double last_sect_inst_readtime,sectInstantInterval;
int N_emission;

struct detectorData{
	int vehType;
	double DataTime;
	double flow;	
	double speed;
	double occupancy;
	double density;
	double instant_flow;
	double instant_speed;
	double instant_density;
};

struct detData{
	int detId;
	double practical_flow;  // used for Onramp only
	int detId_ln[max_onramp_ln];
	double ln_flow[max_onramp_ln];
	
	int sectionId;
	detectorData data[Np]; //
};

struct sectionData{
	int vehType;
	double DataTime;
	double flow;
	double speed;
	double density;
	double Harmonic_speed;
};

struct secData{
	int sectionId;	
	sectionData data[Np];
};

struct data_profile{
	int detId;	
	detectorData data[Np];
};
detData **detection_s;  
detData **detection;	// the realtime data for each cell from detector
detData **detection_onramp;	//the realtime data for onramp from detector
detData **detection_offramp;	//the realtime data for offramp from detector
data_profile *dataProfileUp, *dataProfileDown;	// the predictive(from history) data for the most upstream and downstream cell as the boundary condition
data_profile **dataProfileOnRamp, **dataProfileOffRamp;	// the predictive(from history) data for the onramp and offramp as the boundary condition
secData **section;
data_profile *dataProfile580;

bool moveData(detData* detection);
bool getData(detData* detection,double time,int vehType);
bool storeData(detData* detection);


int opt_metering(void);

bool get_state(double);
double Maxd(double,double);

//add by Dali Wei
double ACC_percent = 0;
double CACC_percent = 0;