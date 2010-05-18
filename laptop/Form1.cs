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
            // stuur de bijbehorende byte (metode uit andere klas)
        }

        private void btVolgendeStap_Click(object sender, EventArgs e)
        {
            // zend signaal dat hij de volgende stap mag uitvoeren 
            // stuur de bijbehoorende byte (methode uit andere klas)
        }

        private void btZendCoordinaten_Click(object sender, EventArgs e)
        {
            // zend de ingevulde coordianten naar de robot

            // lees x,y van persoon
            // stuur een byte
            // lees x,y van wagen
            // stuur een byte
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
