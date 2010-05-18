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

        public Form1()
        {
            InitializeComponent();
            sPort = null;
        }

        private void btKomTerug_Click(object sender, EventArgs e)
        {
            // zend signaal dat robot eigen weg terug vind

            sPort.Write("0-3-48\n"); //robot1
            sPort.Write("0-5-48\n"); //robot2
        }

        private void btVolgendeStap_Click(object sender, EventArgs e)
        {
            // zend signaal dat hij de volgende stap mag uitvoeren
            sPort.Write("0-3-16\n"); //robot1
            sPort.Write("0-5-16\n"); //robot2
        }

        private void btStatus_Click(object sender, EventArgs e)
        {
            // zend signaal dat hij de status geeft 
            sPort.Write("0-3-0\n"); //robot1
            sPort.Write("0-5-0\n"); //robot2
        }

        private void btZendCoordinaten_Click(object sender, EventArgs e)
        {
            

            //strings
            // persoon naar bijde robots 
            string xpersoon1 = "0-2-" + tbPersoonX.Text + "\n";
            string xpersoon2 = "0-4-" + tbPersoonX.Text + "\n";
            string ypersoon1 = "0-2-" + 16 + Convert.ToInt32(tbPersoonY.Text) + "\n";
            string ypersoon2 = "0-4-" + Convert.ToString(16 + Convert.ToInt32(tbPersoonX.Text)) + "\n";
            // robot1
            string xwagen1 = "0-2-" + Convert.ToString(32 + Convert.ToInt32(tbRobot1X.Text)) + "\n";
            string ywagen1 = "0-2-" + Convert.ToString(48 + Convert.ToInt32(tbRobot1Y.Text)) + "\n";
            // robot 2
            string xwagen2 = "0-4-" + Convert.ToString(32 + Convert.ToInt32(tbRobot2X.Text)) + "\n";
            string ywagen2 = "0-4-" + Convert.ToString(48 + Convert.ToInt32(tbRobot2Y.Text)) + "\n";  
   
            // stuur de berichten
            // send string (name) 
            sPort.Write(xpersoon1);
            sPort.Write(xpersoon2);
            sPort.Write(ypersoon1);
            sPort.Write(ypersoon2);
            sPort.Write(xwagen1);
            sPort.Write(ywagen1);
            sPort.Write(xwagen2);
            sPort.Write(ywagen2);
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
        {
            if (sPort == null)
            {
                try
                {
                    sPort = new SerialPort("COM7", 38400, Parity.None, 8, StopBits.One);
                    sPort.DataReceived += new SerialDataReceivedEventHandler(RenesSerialDataReceived);
                    sPort.Open();

                    lbConnect.Text = "wel geconnect";

                }
                catch (System.IO.IOException)
                {
                    lbConnect.Text = "niet geconnect";
                    btKomTerug.Enabled = false;
                    btStatus.Enabled = false;
                    btVolgendeStap.Enabled = false;
                    btZendCoordinaten.Enabled = false;
                }
            }
        }
    }
}
