using System;
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
        public Form1()
        {
            InitializeComponent();
            
        }

        private void btKomTerug_Click(object sender, EventArgs e)
        {
            // zend signaal dat robot eigen weg terug vind

            "0-3-48\n"; //robot1
            "0-5-48\n"; //robot2
        }

        private void btVolgendeStap_Click(object sender, EventArgs e)
        {
            // zend signaal dat hij de volgende stap mag uitvoeren
            "0-3-16\n"; //robot1
            "0-5-16\n"; //robot2
        }

        private void btStatus_Click(object sender, EventArgs e)
        {
            // zend signaal dat hij de status geeft 
            "0-3-0\n"; //robot1
            "0-5-0\n"; //robot2
        }

        private void btZendCoordinaten_Click(object sender, EventArgs e)
        {
            //strings
            // persoon naar bijde robots 
            string xpersoon1 = "0-2-" + tbPersoonX.Text + "\n";
            string xpersoon2 = "0-4-" + tbPersoonX.Text + "\n";
            string ypersoon1 = "0-2-" + Convert.ToString(16 + Convert.ToInt32(tbPersoonY.Text)) + "\n";
            string ypersoon2 = "0-4-" + Convert.ToString(16 + Convert.ToInt32(tbPersoonX.Text)) + "\n";
            // robot1
            string xwagen1 = "0-2-" + Convert.ToString(32 + Convert.ToInt32(tbRobot1X.Text)) + "\n";
            string ywagen1 = "0-2-" + Convert.ToString(48 + Convert.ToInt32(tbRobot1Y.Text)) + "\n";
            // robot 2
            string xwagen2 = "0-4-" + Convert.ToString(32 + Convert.ToInt32(tbRobot2X.Text)) + "\n";
            string ywagen2 = "0-4-" + Convert.ToString(48 + Convert.ToInt32(tbRobot2Y.Text)) + "\n";  
   
            // stuur de berichten
            // send string (name) 
        }

        private void tekendoolhof()
        {
            // ontvang coordinaat
            // driehoek in coordinaat
            // muren trekken langs coordinaat
            // refresh
        }
    }
}
