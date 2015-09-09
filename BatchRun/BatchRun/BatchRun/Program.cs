using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Diagnostics;

namespace BatchRun
{
    class Program
    {
        static void Main(string[] args)
        {
            string line;
            //read txt file to determine how many runs we need
            System.IO.StreamReader file = new System.IO.StreamReader("c:\\CACC_Simu_Data\\Batch_Run.txt");
            line = file.ReadLine();int count =0;
            if(line != null)
            {
                Console.WriteLine("Read flow file");
                Console.WriteLine(line);
                string[] flows = line.Split(',');
                int counter = 0;
                List<double> ramp = new List<double>();
                List<double> through = new List<double>();
                List<double> offramp = new List<double>();
                foreach(string word in flows)
                {
                    if (counter < 3)
                    {
                        ramp.Add(System.Convert.ToInt32(word));
                    }
                    else if (counter < 6)
                    {
                        through.Add(System.Convert.ToInt32(word));
                    }
                    else
                    {
                        offramp.Add(System.Convert.ToInt32(word));
                    }
                    counter++;
                }

                if (ramp[1] == 0) ramp[1] = 1;
                if (offramp[1] == 0) offramp[1] = 1;
                if (through[1] == 0) through[1] = 1;
                count =(int)( Math.Ceiling((ramp[2] - ramp[0]) / ramp[1] )
                    * Math.Ceiling((through[2] - through[0]) / through[1])
                    * Math.Ceiling((offramp[2] - offramp[0]) / offramp[1]));
                if (count <= 0)
                {
                    return;
                }
                else 
                {
                    line = line + ", 0";
                }
            }
            string script = file.ReadLine();
            string script_name = Path.GetFileName(script);
            string path = Path.GetDirectoryName(script);
            string agn_name = file.ReadLine();
            List<string> repids = new List<string>();
            while (true)
            {
                string temp = file.ReadLine();
                if (temp != null)
                    repids.Add(temp);
                else
                    break;
            }
            file.Close();
            if (agn_name != null && repids.Count > 0)
            {
                foreach (string repid in repids)
                {
                    System.IO.StreamWriter tempfile = new System.IO.StreamWriter("c:\\CACC_Simu_Data\\Batch_Volume.txt", false);
                    tempfile.WriteLine(line);
                    tempfile.Close();
                    System.IO.StreamWriter newfile = new System.IO.StreamWriter(path+"\\BatchRun.bat", false);
                    string command = "aconsole.exe -script " +
                        script_name + " " + agn_name + " " + repid;
                    for (int i = 0; i < count; i++)
                    {
                        newfile.WriteLine(command);
                    }
                    newfile.Close();
                    ProcessStartInfo info = new ProcessStartInfo(path + "\\BatchRun.bat");
                    info.WorkingDirectory = path;
                    //info.CreateNoWindow = true;
                    //info.UseShellExecute = false;
                    Process.Start(info);
                }
            }
            else { return; }

            // Suspend the screen.
            Console.ReadLine();
            
        }
    }
}
