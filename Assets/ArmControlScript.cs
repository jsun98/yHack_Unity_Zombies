using UnityEngine;
using System.Collections;
using System.IO.Ports;
using System;

public class ArmControlScript : MonoBehaviour {

    SerialPort stream;
    float yaw, pitch, roll;
    float x, y, z; // accelerations
    double time;
    string[] lastRead;
	// Use this for initialization
	void Start () {
        x = y = z = 0f;
        yaw = pitch = roll = 0f;
        time = 0;
        stream = new SerialPort("COM7", 9600);
        stream.ReadTimeout = 50;
        stream.Open();
    }

    public void WriteToMP(string message)
    {
        stream.WriteLine(message);
        stream.BaseStream.Flush();
    }

    void readAndProcess(int timeout = 0)
    {
        stream.ReadTimeout = timeout;
        try
        {
            string allData = stream.ReadLine();
            process(allData);
        }
        catch (TimeoutException)
        {
            Debug.Log("error: not connected");
            return;
        }
    }
	
    void process(string str)
    {
        string[] data = str.Split(' '); // split by space
        if (data.Length != 10) return;
        if (lastRead == null)
        { // calibration
            lastRead = data;
        }
        // setup values of angles
        double dt = (Convert.ToInt32(data[9]) - Convert.ToInt32(lastRead[9])) * (3.9E-5);
        time += dt;
        roll += (float)(Convert.ToSingle(data[3]) * dt);
        pitch += (float)(Convert.ToSingle(data[4]) * dt);
        yaw += (float)(Convert.ToSingle(data[5]) * dt);

        // get x, y, z values
        x = Convert.ToSingle(data[6]);
        y = Convert.ToSingle(data[7]);
        z = Convert.ToSingle(data[8]);

    }

    // Update is called once per frame
    // gyroscope updates at 64000
    // accelerometer is 36000
    void Update () {
        readAndProcess();
        transform.Rotate(roll, yaw, pitch);

        GetComponent<Rigidbody>().AddForce(x, y, z);
    }
}
