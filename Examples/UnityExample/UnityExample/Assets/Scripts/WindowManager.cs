using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using UnityEngine;

public class WindowManager : MonoBehaviour
{
    private const float TimeBetweenScreenChangeCalculations = 0.016f;
    private float _lastScreenChangeCalculationTime = 0;
    private int prevWidth = 0;
    private int prevHeight = 0;
    private int prevX = -1;
    private int prevY = -1;

    private void Awake()
    {
        _lastScreenChangeCalculationTime = Time.time;
    }

    private void Start()
    {
        Screen.fullScreen = false;
    }

    private void Update()
    {
        if (Time.time - _lastScreenChangeCalculationTime < TimeBetweenScreenChangeCalculations)
            return;

        _lastScreenChangeCalculationTime = Time.time;

        if(Screen.width != prevWidth || Screen.height != prevHeight || Screen.mainWindowPosition.x != prevX || Screen.mainWindowPosition.y != prevY)
        {
            prevWidth = Screen.width;
            prevHeight = Screen.height;
            prevX = Screen.mainWindowPosition.x;
            prevY = Screen.mainWindowPosition.y;
            WindowDimensionMessage message = new WindowDimensionMessage() { 
                mode = "command", 
                data = new WindowDimensionMessage.Data() { 
                    command = "UpdateWindowTransform",
                    x = prevX,
                    y = prevY,
                    width = prevWidth,
                    height = prevHeight
                } };
            string jsonMessage = JsonUtility.ToJson(message);
            Debug.Log(jsonMessage);
            byte[] messageBytes = ASCIIEncoding.ASCII.GetBytes(jsonMessage);
            SubtitleManager.stream.Write(messageBytes, 0, messageBytes.Length);
        }
    }
}
[Serializable]
public class WindowDimensionMessage
{
    [Serializable]
    public class Data
    {
        public string command;
        public int x;
        public int y;
        public int width;
        public int height;
    }
    public string mode;
    public Data data;
}