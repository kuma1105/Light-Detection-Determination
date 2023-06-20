using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO.Ports;
using System;
using System.Threading;//thread 사용을 위해
using System.Threading.Tasks;
using UnityEngine.UI;

public class serialM : MonoBehaviour
{
    public string ports = "COM5";
    private SerialPort nPort;
    private bool check_port_open = false;
    Thread myThread;
    Light directionLight;
    public GameObject lightObject;
    private int lightSensor1;
    private int lightSensor2;
    private int lightSensor3;
    private int lightSensor4;

    public Text ls1;
    public Text ls2;
    public Text ls3;
    public Text ls4;
    public Text solarDirection;

    // Start is called before the first frame update
    void Start()
    {
        directionLight = lightObject.GetComponent<Light>();
    }

    // Update is called once per frame
    void Update()
    {
        printValue();
    }

    private void tryConnectPort()
    {
        nPort = new SerialPort(ports, 115200, Parity.None, 8, StopBits.One);
        nPort.Open();
    }

    public void startPort()
    {
        nPort = new SerialPort(ports, 115200, Parity.None, 8, StopBits.One);
        nPort.Open();
        check_port_open = true;
        myThread = new Thread(new ThreadStart(GetData));
        myThread.Start();
    }

    private void GetData()
    {
        while (myThread.IsAlive)
        {
            try
            {
                if (!nPort.IsOpen)
                {
                    tryConnectPort();
                }
                else
                {
                    string rcv_data = nPort.ReadLine();
                    separateData(rcv_data);
                }
            }
            catch(Exception e)
            {
                Debug.Log("ERROR!!!" + e);
            }
        }
    }

    private void separateData(string data)
    {
        Debug.Log(data);
        char[] sep = { '$', ' ' };
        string[] tmp = data.Split(sep, StringSplitOptions.RemoveEmptyEntries);
        
        lightSensor1 = int.Parse(tmp[0]);
        lightSensor2 = int.Parse(tmp[1]);
        lightSensor3 = int.Parse(tmp[2]);
        lightSensor4 = int.Parse(tmp[3]);
    }

    public int degree=50;
    int threshold = 200; // 해당 구역에 빛이 들어옴을 판단하는 조도센서 기준값
    public void printValue()
    {

        ls1.text = "sensor1: " + lightSensor1;
        ls2.text = "sensor2: " + lightSensor2;
        ls3.text = "sensor3: " + lightSensor3;
        ls4.text = "sensor4: " + lightSensor4;

        if(lightSensor1 >= threshold)
        {
            if(lightSensor2 >= threshold)
            {
                solarDirection.text = "180 degree";
                degree = 180;
            } else if (lightSensor4 >= threshold)
            {
                solarDirection.text = "90 degree";
                degree = 90;
            } else
            {
                solarDirection.text = "135 degree";
                degree = 135;
            }
        }
        else
        {
            if(lightSensor3 >= threshold)
            {
                if (lightSensor2 >= threshold)
                {
                    solarDirection.text = "270 degree";
                    degree = 270;
                }
                else if (lightSensor4 >= threshold)
                {
                    solarDirection.text = "0 degree";
                    degree = 0;
                } else
                {
                    solarDirection.text = "315 degree";
                    degree = 315;
                }
            } else
            {
                if(lightSensor2 >= threshold)
                {
                    solarDirection.text = "225 degree";
                    degree = 225;
                } else
                {
                    solarDirection.text = "45 degree";
                    degree = 45;
                }
            }
        }

        Vector3 rotation = lightObject.transform.rotation.eulerAngles;
        rotation.y = degree;
        lightObject.transform.rotation = Quaternion.Euler(rotation);


        if (lightSensor1 >= 100 & lightSensor2 >= 100 & lightSensor3 >= 100 & lightSensor4 >= 100)
        {
            solarDirection.text = "Direction unknown. Light comes in every part";
        }
    }

    private void OnDestroy()
    {
        if (check_port_open)
        {
            nPort.Close();
            myThread.Abort();
            Debug.Log("port close");
        }
    }
}
