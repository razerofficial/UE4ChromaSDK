using System;
using System.IO;

namespace UnrealEngineVersionChanger
{
    class Program
    {
        public enum Versions
        {
            UNKNOWN,
            UE4_12,
            UE4_15,
            UE4_16,
        };

        private static Versions _sVersion = Versions.UNKNOWN;

        const string TOKEN_MODULE_FIRST = "___HACK_UE4_WANTS_MODULE_FIRST"; //(support 4.15 or below)
        const string TOKEN_HEADER_FIRST = "___HACK_UE4_WANTS_HEADER_FIRST"; //(support 4.16 or above)
        const string TOKEN_GAME_MODE = "___HACK_UE4_WANTS_GAME_MODE"; //(support 4.12)
        const string TOKEN_BASE_GAME_MODE = "___HACK_UE4_WANTS_BASE_GAME_MODE"; //(support above 4.12)
        const string TOKEN_COMMENTS = "//";

        static bool EditLine(ref string contents, ref int i, int lastNewline, string tokenDowngrade, string tokenUpgrade, bool downgrade)
        {
            bool hasChange = false;
            if (contents.Substring(i).StartsWith(tokenDowngrade))
            {
                string firstPart = contents.Substring(0, lastNewline + 1);
                string nextPart = contents.Substring(lastNewline + 1);
                if (downgrade)
                {
                    if (nextPart.StartsWith(TOKEN_COMMENTS))
                    {
                        //remove comments
                        contents = firstPart + nextPart.Substring(TOKEN_COMMENTS.Length);
                        hasChange = true;
                        i = i - TOKEN_COMMENTS.Length + tokenDowngrade.Length;
                    }
                }
                else
                {
                    if (!nextPart.StartsWith(TOKEN_COMMENTS))
                    {
                        //add comments
                        contents = firstPart + TOKEN_COMMENTS + nextPart;
                        hasChange = true;
                        i = i + TOKEN_COMMENTS.Length + tokenDowngrade.Length;
                    }
                }
            }
            if (contents.Substring(i).StartsWith(tokenUpgrade))
            {
                string firstPart = contents.Substring(0, lastNewline + 1);
                string nextPart = contents.Substring(lastNewline + 1);
                if (downgrade)
                {
                    if (!nextPart.StartsWith(TOKEN_COMMENTS))
                    {
                        //add comments
                        contents = firstPart + TOKEN_COMMENTS + nextPart;
                        hasChange = true;
                        i = i + TOKEN_COMMENTS.Length + tokenUpgrade.Length;
                    }
                }
                else
                {
                    if (nextPart.StartsWith(TOKEN_COMMENTS))
                    {
                        //remove comments
                        contents = firstPart + nextPart.Substring(TOKEN_COMMENTS.Length);
                        hasChange = true;
                        i = i - TOKEN_COMMENTS.Length + tokenUpgrade.Length;
                    }
                }
            }
            return hasChange;
        }

        static void EditFile(FileInfo fi)
        {
            try
            {
                Console.WriteLine("Process: {0}", fi.Name);
                string contents = string.Empty;
                using (FileStream fs = File.Open(fi.FullName, FileMode.Open, FileAccess.Read, FileShare.ReadWrite))
                {
                    using (StreamReader sr = new StreamReader(fs))
                    {
                        contents = sr.ReadToEnd();
                    }
                }
                bool hasChange = false;

                int lastNewline = 0;
                for (int i = 0; i < contents.Length; ++i)
                {
                    switch (contents[i])
                    {
                        case '\n':
                        case '\r':
                            lastNewline = i;
                            break;
                    }
                    if (EditLine(ref contents, ref i, lastNewline, TOKEN_MODULE_FIRST, TOKEN_HEADER_FIRST, _sVersion != Versions.UE4_16))
                    {
                        hasChange = true;
                        continue;
                    }
                    if (EditLine(ref contents, ref i, lastNewline, TOKEN_GAME_MODE, TOKEN_BASE_GAME_MODE, _sVersion == Versions.UE4_12))
                    {
                        hasChange = true;
                        continue;
                    }
                }

                if (!hasChange)
                {
                    return;
                }
                if (fi.Exists)
                {
                    fi.Delete();
                }
                using (FileStream fs = File.Open(fi.FullName, FileMode.CreateNew, FileAccess.Write, FileShare.ReadWrite))
                {
                    using (StreamWriter sw = new StreamWriter(fs))
                    {
                        sw.Write(contents);
                        sw.Flush();
                    }
                }
                if (true)
                {
                    //good place to stop
                }
            }
            catch (Exception e)
            {
                Console.WriteLine("Failed to process file: {0} exception={1}", fi.FullName, e);
            }
        }

        static void ProcessFiles(DirectoryInfo di)
        {
            foreach (FileInfo fi in di.GetFiles())
            {
                switch (fi.Extension)
                {
                    case ".h":
                    case ".cpp":
                        EditFile(fi);
                        break;
                }
            }
        }

        static void ProcessFolder(DirectoryInfo di)
        {
            if (di.Name == "Intermediate")
            {
                return;
            }
            //Console.WriteLine("*Process: {0}", di.FullName);

            if (!di.Exists)
            {
                return;
            }

            ProcessFiles(di);

            foreach (DirectoryInfo child in di.GetDirectories())
            {
                if (child == di)
                {
                    continue;
                }
                ProcessFolder(child);
            }
        }
        static void UnitTest()
        {
            // unit test
            EditFile(new FileInfo(@"C:\Public\UE4ChromaSDK\Source\UE4ChromaSDK\UE4ChromaSDKGameModeBase.h"));
        }
        static void Main(string[] args)
        {
            if (args.Length == 0)
            {
                Console.Error.WriteLine("Missing argument!");
                return;
            }

            switch (args[0])
            {
                case "4.12":
                    _sVersion = Versions.UE4_12;
                    break;
                case "4.15":
                    _sVersion = Versions.UE4_15;
                    break;
                case "4.16":
                    _sVersion = Versions.UE4_16;
                    break;
                default:
                    Console.Error.WriteLine("Unsupported version!");
                    return;
            }

            //UnitTest();

            DirectoryInfo di = new DirectoryInfo(Directory.GetCurrentDirectory());
            string targetDir = string.Format("{0}\\Source", di.FullName);
            Console.WriteLine("Processing for {0}... {1}", _sVersion, targetDir);
            ProcessFolder(new DirectoryInfo(targetDir));
            targetDir = string.Format("{0}\\Plugins", di.FullName);
            ProcessFolder(new DirectoryInfo(targetDir));
        }
    }
}
