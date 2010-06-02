using System;
using System.IO.Ports;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace laptop
{
    public partial class Form1 : Form
    {
        private SerialPort sPort;
        public bool IsConnected = false;
        //persoon
        string xpersoon1; 
        string xpersoon2;
        string ypersoon1;
        string ypersoon2;
        // robot1
        string xwagen1;
        string ywagen1;
        // robot 2
        string xwagen2;
        string ywagen2;
        int volgende = 0;

        public Form1()
        {
            InitializeComponent();
            sPort = null;
        }

        public void coordinaten()
        {
            //strings
            xpersoon1 = "0-2-" + tbPersoonX.Text + "\n";
            xpersoon2 = "0-4-" + tbPersoonX.Text + "\n";
            ypersoon1 = "0-2-" + (16 + Convert.ToInt32(tbPersoonY.Text)) + "\n";
            ypersoon2 = "0-4-" + (16 + Convert.ToInt32(tbPersoonY.Text)) + "\n";
            // robot1
            xwagen1 = "0-2-" + (32 + Convert.ToInt32(tbRobot1X.Text)) + "\n";
            ywagen1 = "0-2-" + (48 + Convert.ToInt32(tbRobot1Y.Text)) + "\n";
            // robot 2
            xwagen2 = "0-4-" + (32 + Convert.ToInt32(tbRobot2X.Text)) + "\n";
            ywagen2 = "0-4-" + (48 + Convert.ToInt32(tbRobot2Y.Text)) + "\n";  
   
        }
        private void btZendCoordinaten_Click(object sender, EventArgs e)
        {
            coordinaten();
            switch (volgende)
            {
                case 0: sPort.Write(xpersoon1); 
                    volgende = 1;
                    break;
                case 1: sPort.Write(xpersoon2); 
                    volgende =2;
                    break;
                case 2: sPort.Write(ypersoon1); 
                    volgende=3;
                    break;
                case 3: sPort.Write(ypersoon2); 
                    volgende=4;
                    break;
                case 4: sPort.Write(xwagen1); 
                    volgende=5;
                    break;
                case 5: sPort.Write(ywagen1); 
                    volgende=6;
                    break;
                case 6: sPort.Write(xwagen2); 
                    volgende=7;
                    break;
                case 7: sPort.Write(ywagen2); 
                    volgende=0;
                    btZendCoordinaten.Enabled = false;
                    break;
            }
        }

        private void tekendoolhof()
        {
            // ontvang coordinaat
            // driehoek in coordinaat
            // muren trekken langs coordinaat
            // refresh
        }

        private void RenesSerialDataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            textBox1.Invoke(
                new EventHandler(
                    delegate
                    {
                        textBox1.Text += sPort.ReadExisting();
                        textBox1.Select(textBox1.Text.Length, 0);
                    }));
        }

        private void Form1_Load(object sender, EventArgs e)
        {/*
            if (sPort == null)
            {
                try
                {
                    sPort = new SerialPort("COM8", 38400, Parity.None, 8, StopBits.One);
                    sPort.DataReceived += new SerialDataReceivedEventHandler(RenesSerialDataReceived);
                    sPort.Open();

                    lbConnect.Text = "wel geconnect";

                }
                catch (System.IO.IOException)
                {
                    lbConnect.Text = "niet geconnect";
                    btZendCoordinaten.Enabled = false;
                }
            }*/
        }

        private void schrijfdata()
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            sPort.Write("0-0-32\n");
            
        }

        private void btConnect_Click(object sender, EventArgs e)
        {
            string ComPort = cbComPorts.Text;
            if (sPort == null)
            {
                try
                {
                    sPort = new SerialPort(ComPort, 38400, Parity.None, 8, StopBits.One);
                    sPort.DataReceived += new SerialDataReceivedEventHandler(RenesSerialDataReceived);
                    sPort.Open();
                    lbConnectie.Text = "Connectie gemaakt";
                    IsConnected = true;

                }
                catch (System.IO.IOException)
                {
                    MessageBox.Show("Creation failed");
                    lbConnectie.Text = "Geen Connectie";
                    IsConnected = false;
                }
            }
        }
        public string[] AvailablePorts
        {
            get
            {
                string[] ports = System.IO.Ports.SerialPort.GetPortNames();

                return ports;
            }
        }
        private void updateComports()
        {
            string[] ports = AvailablePorts;

            cbComPorts.Items.Clear();
            for (int i = 0; i < ports.Length; i++)
            {
                cbComPorts.Items.Add(ports[i]);
            }
        }

        private void cbComPorts_MouseClick(object sender, MouseEventArgs e)
        {
            updateComports();
        }

    }
}
