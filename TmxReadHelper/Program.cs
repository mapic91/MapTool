using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net.Sockets;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TmxReadHelper
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            int port = 26543;
            var path = Path.Combine(Application.StartupPath, "PortSetting");
            if (File.Exists(path))
            {
                int.TryParse(File.ReadAllText(path), out port);
            }
            var server = new Server(port);
            if (!server.CheckIsRunning())
            {
                server.Run();
            }
        }
    }
}
