using System.IO;
using UnityEditor;
using UnityEditor.Build;
using UnityEditor.Build.Reporting;
using UnityEngine;

namespace Assets.Scripts
{
    public class PostBuildProcess : ScriptableObject, IPostprocessBuildWithReport
    {
        public int callbackOrder { get { return 0; } }

        public void OnPostprocessBuild(BuildReport report) 
        {
            string outputPath = report.summary.outputPath.Substring(0, report.summary.outputPath.LastIndexOf("UnityExample.exe"));
            string assetPath = Application.dataPath;
            Debug.Log(assetPath + "/Subtitle-Display-Tool.exe");
            Debug.Log(outputPath + "/build/bin/x64/Master/Subtitle-Display-Tool.exe");
            File.Copy(assetPath + "/Subtitle-Display-Tool.exe", outputPath + "/build/bin/x64/Master/Subtitle-Display-Tool.exe");
            File.Copy(assetPath + "/Subtitle-Display-Tool.exe", outputPath + "/build/bin/x64/Debug/Subtitle-Display-Tool.exe");
            File.Copy(assetPath + "/Subtitle-Display-Tool.exe", outputPath + "/build/bin/x64/Release/Subtitle-Display-Tool.exe");
        }


    }
}