using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Diagnostics;
using System.Xml.Serialization;

namespace BatchRun
{
    class Program
    {
        static void Main(string[] args)
        {
            // To read the file, create a FileStream.
            string script = "C:\\CACC_Simu_Data\\batchexample.xml";
            string content = File.ReadAllText(script);
            // Call the Deserialize method and cast to the object type.
            dynamic Ranges = DynamicXml.Parse(content);
            List<List<double>> values = new List<List<double>>();
            int size = Ranges.Range.Count;
            for (int i = 0; i < size; i++ )
            {
                double lower = Math.Round(System.Convert.ToDouble(Ranges.Range[i].low_value),1);
                double high = Math.Round(System.Convert.ToDouble(Ranges.Range[i].high_value),1);
                double increment = Math.Round(System.Convert.ToDouble(Ranges.Range[i].increment), 1);
                values.Add(new List<double>());
                while (lower <= high)
                {
                    values[i].Add(lower);
                    lower += increment;
                    lower = Math.Round(lower, 1);
                }
                if (values[i].Count == 0)
                    values[i].Add(lower);
            }

            string networkname = Ranges.Network;
            string script_name = Ranges.ScriptName;

            List<List<double>> res = new List<List<double>>();
            List<double> temp = new List<double>();
            dfs(values, res, temp, size, 0);

            int repid = 3164;
            string path = Path.GetDirectoryName(script);
            for (int i = 0; i < res.Count; i++)
            {
                string line = "";
                string foldername = "";
                for (int j = 0; j < Ranges.Range.Count; j++)
                {
                    line += System.Convert.ToString(Ranges.Range[j].name)
                        + ":"
                        + res[i][j].ToString()
                        + ",";
                    foldername += res[i][j].ToString();
                }
                System.IO.StreamWriter tempfile = new System.IO.StreamWriter("c:\\CACC_Simu_Data\\ParameterSet.txt", false);
                tempfile.WriteLine(line);
                tempfile.WriteLine(foldername.Replace(".","_"));
                tempfile.Close();

                System.IO.StreamWriter newfile =
                        new System.IO.StreamWriter(path + "\\BatchRun.bat", false);
                string command = "aconsole.exe -script " +
                    script_name + " " + networkname + " " + repid;
                newfile.WriteLine(command);
                newfile.Close();
                
                ProcessStartInfo info = new ProcessStartInfo(path + "\\BatchRun.bat");
                info.WorkingDirectory = path;
                //info.CreateNoWindow = true;
                //info.UseShellExecute = false;
                Process p = Process.Start(info);
                p.WaitForExit();
            }
        }

        private static void dfs(List<List<double>> values, 
            List<List<double>> res, List<double> temp, int size, int index)
        {
            if (temp.Count == size)
            {
                res.Add(temp.ToList());
            }
            else 
            {
                for (int i = 0; i < values[index].Count; i++)
                {
                    temp.Add(values[index][i]);
                    dfs(values, res, temp, size, index + 1);
                    temp.RemoveAt(temp.Count - 1);
                }
            }
        }
    }
}


