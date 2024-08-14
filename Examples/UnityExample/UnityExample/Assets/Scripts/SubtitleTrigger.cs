using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using UnityEngine;

public class SubtitleTrigger : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    void OnTriggerEnter(Collider other)
    {
        AdvancedMessage message = new AdvancedMessage() { mode = "simple", data = new AdvancedMessage.Data() { dialogue = "test string" } };
        string jsonMessage = JsonUtility.ToJson(message);
        byte[] messageBytes = ASCIIEncoding.ASCII.GetBytes(jsonMessage);
        SubtitleManager.stream.Write(messageBytes, 0, messageBytes.Length);
    }
}
[Serializable]
public class AdvancedMessage
{
    [Serializable]
    public class Data
    {
        public string dialogue;
    }
    public string mode;
    public Data data;


}