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
        //variabelen
        int countCommandDH = 0; // Telt hoeveel commando's er voor het doolhof binnen komen
        int[] commandDoolhof = new int[4]; // Array wat gevult word met 4 comando's die nodig 
        string[] commandSerialPort = new string[400]; //Alle binnen komende comando's worden eerst verwerkt in deze Array van Strings 
        //zijn voor het opslaan van 1 stap inhet doolhof
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
        int eerstVolgendePlekinArray = 0;

        public Form1()
        {
            InitializeComponent();
            this.Text = "RoboHeroes - Geen Connectie";
            sPort = null;

            //vul array
            commandSerialPort[0] = "0-0-1";
            commandSerialPort[1] = "0-0-16";
            commandSerialPort[2] = "0-0-35";
            commandSerialPort[3] = "0-0-47";
            commandSerialPort[4] = "0-0-4";
            commandSerialPort[5] = "0-0-16";
            commandSerialPort[6] = "0-0-35";
            commandSerialPort[7] = "0-0-47";
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
                    volgende = 2;
                    break;
                case 2: sPort.Write(ypersoon1);
                    volgende = 3;
                    break;
                case 3: sPort.Write(ypersoon2);
                    volgende = 4;
                    break;
                case 4: sPort.Write(xwagen1);
                    volgende = 5;
                    break;
                case 5: sPort.Write(ywagen1);
                    volgende = 6;
                    break;
                case 6: sPort.Write(xwagen2);
                    volgende = 7;
                    break;
                case 7: sPort.Write(ywagen2);
                    volgende = 0;
                    btZendCoordinaten.Enabled = false;
                    break;
            }
        }



        private void RenesSerialDataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            textBox1.Invoke(
                new EventHandler(
                    delegate
                    {

                        textBox1.Clear();
                        textBox1.Text += sPort.ReadExisting();
                        textBox1.Select(textBox1.Text.Length, 0);
                        schrijfdataNaarArray(textBox1.Text);

                    }));
        }

        private void schrijfdataNaarArray(string bericht)
        {
            try
            {
                bericht = bericht.Remove(bericht.Length - 1, 1);

                string l2charBericht = bericht.Substring(bericht.Length - 2, 2);

                if (l2charBericht != "50")
                {
                    if (bericht.Length < 8)
                    {
                        commandSerialPort[eerstVolgendePlekinArray] = bericht;
                        eerstVolgendePlekinArray++;
                    }
                }
                else
                {
                    int countStappen = 0;
                    int countPlekArray = 0;

                    for (int i = 0; i < commandSerialPort.Length; i++)
                    {
                        countStappen++;

                        if (countStappen == 4)
                        {

                            TekenDoolhof(commandSerialPort[0 + countPlekArray], commandSerialPort[1 + countPlekArray],
                                commandSerialPort[2 + countPlekArray], commandSerialPort[3 + countPlekArray]);

                            countPlekArray += 4;
                            countStappen = 0;
                        }

                    }


                    /*
                    int countStappen = 0;
                    int countPlekArray = 0;

                    for(int i=0; i< commandSerialPort.Length;i++) 
                    {
                        while(commandSerialPort[i] != "83")
                        {
                            if (countPlekArray < 5)
                            {
                                TekenDoolhof(commandSerialPort[countPlekArray + i]);




                            }
                        }
                    }*/

                }
            }
            catch (ArgumentOutOfRangeException)
            { }
            catch (FormatException)
            { }
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
                    this.Text = "RoboHeroes - Connectie Werkt via " + sPort.PortName;
                    IsConnected = true;

                }
                catch (System.IO.IOException)
                {
                    MessageBox.Show("Creation failed");
                    this.Text = "RoboHeroes - Geen Connectie";
                    IsConnected = false;
                }
                catch (ArgumentException)
                {
                    MessageBox.Show("Selecteer eerst een COM poort");
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





        private void groupBox2_Paint(object sender, PaintEventArgs e)
        {//begin paint event





        }//end paint event

        private void btTest_Click(object sender, EventArgs e)
        {
            TekenDoolhof(tbtest1.Text, tbtest2.Text, tbtest3.Text, tbtest4.Text);
            //Graphics graphics = CreateGraphics();
            //Pen doolhofPen = new Pen(Color.Green, 3);
            //int gbX = gbDoolhof.Location.X;
            // int gbY = gbDoolhof.Location.Y;

            //graphics.DrawLine(doolhofPen, (500), (50), (500), (100));


        }

        /*********************TekenDoolhof methode********************************/
        public void TekenDoolhof(string Xpos, string Ypos, string richNOZW, string muren)
        {//methode doolhof
            Graphics graphics = CreateGraphics();
            Pen doolhofPen = new Pen(Color.Green, 3);
            int Xpositie;
            int Ypositie;
            int murenLVR;
            int richtingNOZW;


            if (Xpos != null && Ypos != null && richNOZW != null && muren != null)
            {
                string l2charsXpos = Xpos.Substring(Xpos.Length - 2);
                string l2charsYpos = Ypos.Substring(Ypos.Length - 2);
                string l2charsRichting = richNOZW.Substring(richNOZW.Length - 2);
                string l2charsMuren = muren.Substring(muren.Length - 2);

                if (l2charsXpos.IndexOf("-") != -1) { Xpositie = Convert.ToInt32(l2charsXpos.Remove(0, 1)); }
                else{Xpositie = Convert.ToInt32(l2charsXpos);}

                if (l2charsYpos.IndexOf("-") != -1) {Ypositie = (Convert.ToInt32(l2charsYpos.Remove(0, 1))-16); }
                else{Ypositie = ((Convert.ToInt32(l2charsYpos))-16);}

                if (l2charsRichting.IndexOf("-") != -1) { richtingNOZW = Convert.ToInt32(l2charsRichting.Remove(0, 1)); }
                else { richtingNOZW = Convert.ToInt32(l2charsRichting); }

                if (l2charsMuren.IndexOf("-") != -1) { murenLVR = Convert.ToInt32(l2charsMuren.Remove(0, 1)); }
                else { murenLVR = Convert.ToInt32(l2charsMuren); }



                if (murenLVR == 40) //0 == geen muren gezien
                {

                }
                if (murenLVR == 41) //1 == Muur rechts   /*Getest*/
                {
                    //Noord
                    if (richtingNOZW == 32) { graphics.DrawLine(doolhofPen, ((434) + ((Xpositie + 1) * 40)), ((77) + (Ypositie * 40)), ((434) + (((Xpositie + 1)) * 40)), ((77) + ((Ypositie + 1) * 40))); }
                    //Oost
                    if (richtingNOZW == 33) { graphics.DrawLine(doolhofPen, (434 + (Xpositie * 40)), 77 + ((Ypositie + 1) * 40), 434 + ((Xpositie + 1) * 40), 77 + ((Ypositie + 1) * 40)); }
                    //Zuid
                    if (richtingNOZW == 34) { graphics.DrawLine(doolhofPen, (434 + (Xpositie * 40)), 77 + ((Ypositie + 1) * 40), 434 + (Xpositie * 40), 77 + ((Ypositie) * 40)); }
                    //West
                    if (richtingNOZW == 35) { graphics.DrawLine(doolhofPen, (434 + (Xpositie * 40)), 77 + ((Ypositie) * 40), 434 + ((Xpositie + 1) * 40), 77 + ((Ypositie) * 40)); }


                }

                if (murenLVR == 42)//2 == Muur voor  /*Getest*/
                {
                    //noord
                    if (richtingNOZW == 32) { graphics.DrawLine(doolhofPen, (434 + (Xpositie * 40)), 77 + ((Ypositie) * 40), 434 + ((Xpositie + 1) * 40), 77 + ((Ypositie) * 40)); }
                    //Oost
                    if (richtingNOZW == 33) { graphics.DrawLine(doolhofPen, ((434) + ((Xpositie + 1) * 40)), ((77) + (Ypositie * 40)), ((434) + (((Xpositie + 1)) * 40)), ((77) + ((Ypositie + 1) * 40))); }
                    //Zuid
                    if (richtingNOZW == 34) { graphics.DrawLine(doolhofPen, (434 + (Xpositie * 40)), 77 + ((Ypositie + 1) * 40), 434 + ((Xpositie + 1) * 40), 77 + ((Ypositie + 1) * 40)); }
                    //West
                    if (richtingNOZW == 35) { graphics.DrawLine(doolhofPen, (434 + (Xpositie * 40)), 77 + ((Ypositie + 1) * 40), 434 + (Xpositie * 40), 77 + ((Ypositie) * 40)); }



                }
                if (murenLVR == 43)//3 == Muur voor & rechts /*Getest*/
                {
                    //Noord
                    if (richtingNOZW == 32) { graphics.DrawLine(doolhofPen, ((434) + ((Xpositie + 1) * 40)), ((77) + (Ypositie * 40)), ((434) + (((Xpositie + 1)) * 40)), ((77) + ((Ypositie + 1) * 40))); }
                    if (richtingNOZW == 32) { graphics.DrawLine(doolhofPen, (434 + (Xpositie * 40)), 77 + ((Ypositie) * 40), 434 + ((Xpositie + 1) * 40), 77 + ((Ypositie) * 40)); }
                    //Oost
                    if (richtingNOZW == 33) { graphics.DrawLine(doolhofPen, (434 + (Xpositie * 40)), 77 + ((Ypositie + 1) * 40), 434 + ((Xpositie + 1) * 40), 77 + ((Ypositie + 1) * 40)); }
                    if (richtingNOZW == 33) { graphics.DrawLine(doolhofPen, ((434) + ((Xpositie + 1) * 40)), ((77) + (Ypositie * 40)), ((434) + (((Xpositie + 1)) * 40)), ((77) + ((Ypositie + 1) * 40))); }
                    //Zuid
                    if (richtingNOZW == 34) { graphics.DrawLine(doolhofPen, (434 + (Xpositie * 40)), 77 + ((Ypositie + 1) * 40), 434 + (Xpositie * 40), 77 + ((Ypositie) * 40)); }
                    if (richtingNOZW == 34) { graphics.DrawLine(doolhofPen, (434 + (Xpositie * 40)), 77 + ((Ypositie + 1) * 40), 434 + ((Xpositie + 1) * 40), 77 + ((Ypositie + 1) * 40)); }
                    //West
                    if (richtingNOZW == 35) { graphics.DrawLine(doolhofPen, (434 + (Xpositie * 40)), 77 + ((Ypositie) * 40), 434 + ((Xpositie + 1) * 40), 77 + ((Ypositie) * 40)); }
                    if (richtingNOZW == 35) { graphics.DrawLine(doolhofPen, (434 + (Xpositie * 40)), 77 + ((Ypositie + 1) * 40), 434 + (Xpositie * 40), 77 + ((Ypositie) * 40)); }


                }
                if (murenLVR == 44)//4 == Muur links  /*Getest*/
                {
                    //Noord
                    if (richtingNOZW == 32) { graphics.DrawLine(doolhofPen, (434 + (Xpositie * 40)), 77 + ((Ypositie + 1) * 40), 434 + (Xpositie * 40), 77 + ((Ypositie) * 40)); }
                    //Oost
                    if (richtingNOZW == 33) { graphics.DrawLine(doolhofPen, (434 + (Xpositie * 40)), 77 + ((Ypositie) * 40), 434 + ((Xpositie + 1) * 40), 77 + ((Ypositie) * 40)); }
                    //Zuid
                    if (richtingNOZW == 34) { graphics.DrawLine(doolhofPen, ((434) + ((Xpositie + 1) * 40)), ((77) + (Ypositie * 40)), ((434) + (((Xpositie + 1)) * 40)), ((77) + ((Ypositie + 1) * 40))); }
                    //West
                    if (richtingNOZW == 35) { graphics.DrawLine(doolhofPen, (434 + (Xpositie * 40)), 77 + ((Ypositie + 1) * 40), 434 + ((Xpositie + 1) * 40), 77 + ((Ypositie + 1) * 40)); }


                }
                if (murenLVR == 45)//5 == Muur links & recht /*Getest*/
                {
                    //Noord
                    if (richtingNOZW == 32) { graphics.DrawLine(doolhofPen, (434 + (Xpositie * 40)), 77 + ((Ypositie + 1) * 40), 434 + (Xpositie * 40), 77 + ((Ypositie) * 40)); }
                    if (richtingNOZW == 32) { graphics.DrawLine(doolhofPen, ((434) + ((Xpositie + 1) * 40)), ((77) + (Ypositie * 40)), ((434) + (((Xpositie + 1)) * 40)), ((77) + ((Ypositie + 1) * 40))); }
                    //Oost
                    if (richtingNOZW == 33) { graphics.DrawLine(doolhofPen, (434 + (Xpositie * 40)), 77 + ((Ypositie) * 40), 434 + ((Xpositie + 1) * 40), 77 + ((Ypositie) * 40)); }
                    if (richtingNOZW == 33) { graphics.DrawLine(doolhofPen, (434 + (Xpositie * 40)), 77 + ((Ypositie + 1) * 40), 434 + ((Xpositie + 1) * 40), 77 + ((Ypositie + 1) * 40)); }
                    //Zuid
                    if (richtingNOZW == 34) { graphics.DrawLine(doolhofPen, ((434) + ((Xpositie + 1) * 40)), ((77) + (Ypositie * 40)), ((434) + (((Xpositie + 1)) * 40)), ((77) + ((Ypositie + 1) * 40))); }
                    if (richtingNOZW == 34) { graphics.DrawLine(doolhofPen, (434 + (Xpositie * 40)), 77 + ((Ypositie + 1) * 40), 434 + (Xpositie * 40), 77 + ((Ypositie) * 40)); }
                    //West
                    if (richtingNOZW == 35) { graphics.DrawLine(doolhofPen, (434 + (Xpositie * 40)), 77 + ((Ypositie + 1) * 40), 434 + ((Xpositie + 1) * 40), 77 + ((Ypositie + 1) * 40)); }
                    if (richtingNOZW == 35) { graphics.DrawLine(doolhofPen, (434 + (Xpositie * 40)), 77 + ((Ypositie) * 40), 434 + ((Xpositie + 1) * 40), 77 + ((Ypositie) * 40)); }


                }
                if (murenLVR == 46)//6 == Muur links & voor
                {
                    //Noord
                    if (richtingNOZW == 32) { graphics.DrawLine(doolhofPen, (434 + (Xpositie * 40)), 77 + ((Ypositie + 1) * 40), 434 + (Xpositie * 40), 77 + ((Ypositie) * 40)); }
                    if (richtingNOZW == 32) { graphics.DrawLine(doolhofPen, (434 + (Xpositie * 40)), 77 + ((Ypositie) * 40), 434 + ((Xpositie + 1) * 40), 77 + ((Ypositie) * 40)); }
                    //Oost
                    if (richtingNOZW == 33) { graphics.DrawLine(doolhofPen, (434 + (Xpositie * 40)), 77 + ((Ypositie) * 40), 434 + ((Xpositie + 1) * 40), 77 + ((Ypositie) * 40)); }
                    if (richtingNOZW == 33) { graphics.DrawLine(doolhofPen, ((434) + ((Xpositie + 1) * 40)), ((77) + (Ypositie * 40)), ((434) + (((Xpositie + 1)) * 40)), ((77) + ((Ypositie + 1) * 40))); }
                    //Zuid
                    if (richtingNOZW == 34) { graphics.DrawLine(doolhofPen, ((434) + ((Xpositie + 1) * 40)), ((77) + (Ypositie * 40)), ((434) + (((Xpositie + 1)) * 40)), ((77) + ((Ypositie + 1) * 40))); }
                    if (richtingNOZW == 34) { graphics.DrawLine(doolhofPen, (434 + (Xpositie * 40)), 77 + ((Ypositie + 1) * 40), 434 + ((Xpositie + 1) * 40), 77 + ((Ypositie + 1) * 40)); }
                    //West
                    if (richtingNOZW == 35) { graphics.DrawLine(doolhofPen, (434 + (Xpositie * 40)), 77 + ((Ypositie + 1) * 40), 434 + ((Xpositie + 1) * 40), 77 + ((Ypositie + 1) * 40)); }
                    if (richtingNOZW == 35) { graphics.DrawLine(doolhofPen, (434 + (Xpositie * 40)), 77 + ((Ypositie + 1) * 40), 434 + (Xpositie * 40), 77 + ((Ypositie) * 40)); }

                }
                if (murenLVR == 47)//7 == Muur links & voor & rechts
                {
                    //Noord
                    if (richtingNOZW == 32) { graphics.DrawLine(doolhofPen, (434 + (Xpositie * 40)), 77 + ((Ypositie + 1) * 40), 434 + (Xpositie * 40), 77 + ((Ypositie) * 40)); }
                    if (richtingNOZW == 32) { graphics.DrawLine(doolhofPen, (434 + (Xpositie * 40)), 77 + ((Ypositie) * 40), 434 + ((Xpositie + 1) * 40), 77 + ((Ypositie) * 40)); }
                    if (richtingNOZW == 32) { graphics.DrawLine(doolhofPen, ((434) + ((Xpositie + 1) * 40)), ((77) + (Ypositie * 40)), ((434) + (((Xpositie + 1)) * 40)), ((77) + ((Ypositie + 1) * 40))); }
                    //Oost
                    if (richtingNOZW == 33) { graphics.DrawLine(doolhofPen, (434 + (Xpositie * 40)), 77 + ((Ypositie) * 40), 434 + ((Xpositie + 1) * 40), 77 + ((Ypositie) * 40)); }
                    if (richtingNOZW == 33) { graphics.DrawLine(doolhofPen, ((434) + ((Xpositie + 1) * 40)), ((77) + (Ypositie * 40)), ((434) + (((Xpositie + 1)) * 40)), ((77) + ((Ypositie + 1) * 40))); }
                    if (richtingNOZW == 33) { graphics.DrawLine(doolhofPen, (434 + (Xpositie * 40)), 77 + ((Ypositie + 1) * 40), 434 + ((Xpositie + 1) * 40), 77 + ((Ypositie + 1) * 40)); }
                    //Zuid
                    if (richtingNOZW == 34) { graphics.DrawLine(doolhofPen, ((434) + ((Xpositie + 1) * 40)), ((77) + (Ypositie * 40)), ((434) + (((Xpositie + 1)) * 40)), ((77) + ((Ypositie + 1) * 40))); }
                    if (richtingNOZW == 34) { graphics.DrawLine(doolhofPen, (434 + (Xpositie * 40)), 77 + ((Ypositie + 1) * 40), 434 + ((Xpositie + 1) * 40), 77 + ((Ypositie + 1) * 40)); }
                    if (richtingNOZW == 34) { graphics.DrawLine(doolhofPen, (434 + (Xpositie * 40)), 77 + ((Ypositie + 1) * 40), 434 + (Xpositie * 40), 77 + ((Ypositie) * 40)); }
                    //West
                    if (richtingNOZW == 35) { graphics.DrawLine(doolhofPen, (434 + (Xpositie * 40)), 77 + ((Ypositie + 1) * 40), 434 + ((Xpositie + 1) * 40), 77 + ((Ypositie + 1) * 40)); }
                    if (richtingNOZW == 35) { graphics.DrawLine(doolhofPen, (434 + (Xpositie * 40)), 77 + ((Ypositie + 1) * 40), 434 + (Xpositie * 40), 77 + ((Ypositie) * 40)); }
                    if (richtingNOZW == 35) { graphics.DrawLine(doolhofPen, (434 + (Xpositie * 40)), 77 + ((Ypositie) * 40), 434 + ((Xpositie + 1) * 40), 77 + ((Ypositie) * 40)); }
                }
                /*if (commandDoolhof[3] != null)
                {

                    MessageBox.Show("blalalkal");
                    countCommandDH = 0;
                    commandDoolhof[0] = '\0';
                    commandDoolhof[1] = '\0';
                    commandDoolhof[2] = '\0';
                    commandDoolhof[3] = '\0';
                }*/
            }
        }

        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            /*************Tekendoolhof****************************/
            Graphics graphics = e.Graphics;
            Pen doolhofPen = new Pen(Color.Green, 3);
            Pen myPen = new Pen(Color.Blue, 3);

            int X = 394;
            int Y = 37;
            graphics.FillRectangle(Brushes.White, 40 + X, 40 + Y, 400, 400);
            //horizontaal 
            graphics.DrawLine(myPen, 39 + X, 40 + Y, 442 + X, 40 + Y);
            graphics.DrawLine(Pens.Black, 40 + X, 80 + Y, 440 + X, 80 + Y);
            graphics.DrawLine(Pens.Black, 40 + X, 120 + Y, 440 + X, 120 + Y);
            graphics.DrawLine(Pens.Black, 40 + X, 160 + Y, 440 + X, 160 + Y);
            graphics.DrawLine(Pens.Black, 40 + X, 200 + Y, 440 + X, 200 + Y);
            graphics.DrawLine(Pens.Black, 40 + X, 240 + Y, 440 + X, 240 + Y);
            graphics.DrawLine(Pens.Black, 40 + X, 280 + Y, 440 + X, 280 + Y);
            graphics.DrawLine(Pens.Black, 40 + X, 320 + Y, 440 + X, 320 + Y);
            graphics.DrawLine(Pens.Black, 40 + X, 360 + Y, 440 + X, 360 + Y);
            graphics.DrawLine(Pens.Black, 40 + X, 400 + Y, 440 + X, 400 + Y);
            graphics.DrawLine(myPen, 39 + X, 440 + Y, 442 + X, 440 + Y);

            //vertikaal
            graphics.DrawLine(myPen, 40 + X, 40 + Y, 40 + X, 440 + Y);
            graphics.DrawLine(Pens.Black, 80 + X, 40 + Y, 80 + X, 440 + Y);
            graphics.DrawLine(Pens.Black, 120 + X, 40 + Y, 120 + X, 440 + Y);
            graphics.DrawLine(Pens.Black, 160 + X, 40 + Y, 160 + X, 440 + Y);
            graphics.DrawLine(Pens.Black, 200 + X, 40 + Y, 200 + X, 440 + Y);
            graphics.DrawLine(Pens.Black, 240 + X, 40 + Y, 240 + X, 440 + Y);
            graphics.DrawLine(Pens.Black, 280 + X, 40 + Y, 280 + X, 440 + Y);
            graphics.DrawLine(Pens.Black, 320 + X, 40 + Y, 320 + X, 440 + Y);
            graphics.DrawLine(Pens.Black, 360 + X, 40 + Y, 360 + X, 440 + Y);
            graphics.DrawLine(Pens.Black, 400 + X, 40 + Y, 400 + X, 440 + Y);
            graphics.DrawLine(myPen, 440 + X, 40 + Y, 440 + X, 440 + Y);
            /*************Tekendoolhof****************************/
        }


    }
}



