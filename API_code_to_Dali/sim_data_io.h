/***********************************************************************

     Include some of the data IO file open and close

************************************************************************/

#include <string>
#include <windows.h>
#include <sys/stat.h>
#include <direct.h>
FILE *dbg_f, *dmd_f, *rm_f,  *sec_outfile, *pp, *cfg_file; //*det_outfile,

bool dirExists(const std::string& dirName_in)
{
	  DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	  if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;  //something is wrong with your path!

	  if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;   // this is a directory!

	  return false;    // this is not a directory!
}

void createbase(int replication, int acc, int cacc)
{
	char str_tmp1[len_str] = "a"; 
	sprintf_s(str_tmp1,len_str, "C:\\CACC_Simu_Data\\acc%u_cacc%u", acc, cacc);
	if (dirExists(str_tmp1) == false)
	{
		_mkdir(str_tmp1);
	}
}

void createreplicate(int replication, int acc, int cacc)
{
	char str_tmp[len_str] = "a"; 
	sprintf_s(str_tmp,len_str, "C:\\CACC_Simu_Data\\acc%u_cacc%u\\%u", acc, cacc, replication);
	if (dirExists(str_tmp) == false)
	{
		_mkdir(str_tmp);
 	}
}

void createdetector(int replication, int acc, int cacc)
{
	char str_tmp2[len_str] = "a"; 
	sprintf_s(str_tmp2,len_str, "C:\\CACC_Simu_Data\\acc%u_cacc%u\\%u\\detector", acc, cacc, replication);
	if (dirExists(str_tmp2) == false)
	{
		_mkdir(str_tmp2);
 	}
}

void CheckorCreateDirs(int replication, int acc, int cacc)
{	
	createbase(replication, acc, cacc);
	createreplicate(replication, acc, cacc);
	createdetector(replication, acc, cacc);
}


/*void CheckorCreateDirs(int replication, int acc, int cacc)
{	
	char str_tmp1[len_str] = "a"; 
	sprintf_s(str_tmp1,len_str, "C:\\CACC_Simu_Data\\acc%u_cacc%u", acc, cacc);
	if (dirExists(str_tmp1) == false)
	{
		_mkdir(str_tmp1);
	}
	
	char str_tmp[len_str] = "a"; 
	sprintf_s(str_tmp,len_str, "C:\\CACC_Simu_Data\\acc%u_cacc%u\\%u", acc, cacc, replication);
	if (dirExists(str_tmp) == false)
	{
		_mkdir(str_tmp);
 	}
}*/

int Init_sim_data_out(double acc_percent, double cacc_percent)
{
	errno_t err;
	
	char dbg_filename[len_str];
	char dmd_filename[len_str];
	char posted_filename[len_str];
	char sec_filename[len_str];


	CheckorCreateDirs(replication, (int)(acc_percent*100), (int)(cacc_percent*100));

	/*if (use_RM == 0)                                         
		sprintf_s(dbg_filename,len_str, "C:\\CACC_Simu_Data\\acc%u_cacc%u\\%u\\dbg_file.txt",
					(int)(acc_percent*100), (int)(cacc_percent*100),replication);*/
	
	/*err=fopen_s(&dbg_f,dbg_filename,"w+");
	if (err != 0)
		AKIPrintString("dbg_file not open!");
		
	if (use_RM == 0)    
		sprintf_s(dmd_filename,len_str, "C:\\CACC_Simu_Data\\acc%u_cacc%u\\%u\\dmd_file.txt",
					(int)(acc_percent*100), (int)(cacc_percent*100),replication);
	                                    
	err=fopen_s(&dmd_f,dmd_filename,"w+");
	if (err != 0)		
		AKIPrintString("dmd_file not open!");*/
	
	/*if (use_RM == 0)
		sprintf_s(posted_filename,len_str, "C:\\CACC_Simu_Data\\acc%u_cacc%u\\%u\\rm_speed.txt",
					(int)(acc_percent*100), (int)(cacc_percent*100),replication);
	
		err=fopen_s(&rm_f,posted_filename,"w+");
	if (err != 0)		
		AKIPrintString("posted_speed_file not open!");
	
	if (use_RM == 0)
		sprintf_s(sec_filename,len_str, "C:\\CACC_Simu_Data\\acc%u_cacc%u\\%u\\section.txt",
					(int)(acc_percent*100), (int)(cacc_percent*100),replication);
	
	err=fopen_s(&sec_outfile,sec_filename,"w+");
	if(err!=0)
		AKIPrintString("can not open file");*/

	return 1;
}
bool Init()  // A major function; Called by AAPI.cxx: the top function for intialization of the whole system
{
	int i; 

	double tmp_buff=0.0;

	for(i=0;i<SecSize;i++)
	{
		u[i]=89.0; 		
	}
	for(i=0;i<SecSize;i++)
	{
		v[i]=0.0;
		q[i]=0.0;
		o[i]=0.0;
	}

	memset(&ln_metering_rate,0,sizeof(ln_metering_rate));
	memset(&dmd,0,sizeof(dmd));
	memset(&s,0,sizeof(s));
	memset(&qc,0,sizeof(qc));
	memset(&q_main,0,sizeof(q_main));
	memset(&dyna_min_r,0,sizeof(dyna_min_r));
	memset(&dyna_max_r,0,sizeof(dyna_max_r));
	memset(&release_cycle,0,sizeof(release_cycle));
	memset(&RM_occ,0,sizeof(RM_occ));
	memset(&Ramp_rt,0,sizeof(Ramp_rt));
	
	for(i=0;i<NumOnRamp;i++)
	{
		dmd[i]=0.0;
		dyna_min_r[i]=200.0;
		dyna_max_r[i]=dyna_min_r[i]+rm_band;	
		Ramp_rt[i]=max_RM_rt;   // only used in Set_Coord_ALINEA
	}
	
	// read system parameters
	
	replication=ANGConnGetReplicationId();
	sectionNum=AKIInfNetNbSectionsANG();
	detectorNum=AKIDetGetNumberDetectors();
	centroidNum=AKIInfNetNbCentroids();
	detInterval=AKIDetGetIntervalDetection();  // This line of code caused crash; 3/4/14
	N_emission=AKIEstGetNbPollutants();


	T=detInterval/3600.0;
	if(AKIIsGatheringStatistics()<=0)
		AKIPrintString("statistics data is not gathering or error");
	if (detInterval!=AKIEstGetIntervalStatistics())
		AKIPrintString("detector interval does not equal to statistics interval");
	
	
	InitRealtimeDetection();	// mempry allocation; for detection measure
	InitRealtimeDetection_s();	// memory allocation; for control detection, almost the same as InitRealtimeDetection(), just has less detector and not save data
    
	InitRealTimeSection();	//for section measure
	
	
	
	u[NumOnRamp]=104;   // Dim Ok	
	ControlOn=false;
	StateOn=0;
	StateOff=0;
	
	int id;
	int ori=0, dest=0;	
	A2KCentroidInf ceninfo;

	for(i=0;i<centroidNum;i++)
	{
		id=AKIInfNetGetCentroidId(i);
		ceninfo=AKIInfNetGetCentroidInf(id);
		if(ceninfo.report!=0)
			AKIPrintString("centroid error");
		if(ceninfo.IsOrigin==true)
		{
			origin[ori]=id;
			ori++;
		}
		if(ceninfo.IsDestination==true)
		{
			destination[dest]=id;
			dest++;
		}
	}

	

	return true;
}



int Finish_sim_data_out()
{
	/*fflush(dbg_f);
	fclose(dbg_f);*/
	/*fflush(dmd_f);
	fclose(dmd_f);*/	
	/*fflush(sec_outfile);
	fclose(sec_outfile);*/

	return 1;
}