using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Net;
using System.Net.Sockets;
using UnityEngine;

public class SubtitleManager : MonoBehaviour
{
    static public int portNumber = 9999;
    static public TcpClient socket;
    static public NetworkStream stream;
    private byte[] receiveBuffer;
    private int dataBufferSize = 10240;
    private void Start()
    {
        if(Application.isEditor)
        {
            Process.Start((Application.dataPath + "\\Subtitle-Display-Tool.exe").Replace('/', '\\'), portNumber.ToString());
        } else
        {
            Process.Start("Subtitle-Display-Tool.exe", portNumber.ToString());
        }
        socket = new TcpClient("127.0.0.1", 9999);
        stream = socket.GetStream();
    }

    private void ConnectCallback(IAsyncResult result)
    {
        socket.EndConnect(result);
        if (!socket.Connected) return;
        // stream is a NetworkStream you should declare somewhere
        stream = socket.GetStream();
        // receiveBuffer is a byte[] you should declare somewhere
        receiveBuffer = new byte[dataBufferSize];
        stream.BeginRead(receiveBuffer, 0, dataBufferSize, ReceiveCallback, null);
    }

    private void ReceiveCallback(IAsyncResult result)
    {
        return;
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
