using System;
using System.IO;

namespace UnrealEngineVersionChanger
{
    class Program
    {
        private static bool _sDowngrade = false;

        const string TOKEN_MODULE_FIRST = "___HACK_UE4_WANTS_MODULE_FIRST";
        const string TOKEN_HEADER_FIRST = "___HACK_UE4_WANTS_HEADER_FIRST";
        const string TOKEN_COMMENTS = "//";

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
                    if (contents.Substring(i).StartsWith(TOKEN_MODULE_FIRST))
                    {
                        string firstPart = contents.Substring(0, lastNewline+1);
                        string nextPart = contents.Substring(lastNewline + 1);
                        if (_sDowngrade)
                        {
                            if (nextPart.StartsWith(TOKEN_COMMENTS))
                            {
                                //remove comments
                                contents = firstPart + nextPart.Substring(TOKEN_COMMENTS.Length);
                                hasChange = true;
                                i = i - TOKEN_COMMENTS.Length + TOKEN_MODULE_FIRST.Length;
                                continue;
                            }
                        }
                        else
                        {
                            if (!nextPart.StartsWith(TOKEN_COMMENTS))
                            {
                                //add comments
                                contents = firstPart + TOKEN_COMMENTS + nextPart;
                                hasChange = true;
                                i = i + TOKEN_COMMENTS.Length + TOKEN_MODULE_FIRST.Length;
                                continue;
                            }
                        }
                    }
                    if (contents.Substring(i).StartsWith(TOKEN_HEADER_FIRST))
                    {
                        string firstPart = contents.Substring(0, lastNewline + 1);
                        string nextPart = contents.Substring(lastNewline + 1);
                        if (_sDowngrade)
                        {
                            if (!nextPart.StartsWith(TOKEN_COMMENTS))
                            {
                                //add comments
                                contents = firstPart + TOKEN_COMMENTS + nextPart;
                                hasChange = true;
                                i = i + TOKEN_COMMENTS.Length + TOKEN_HEADER_FIRST.Length;
                                continue;
                            }
                        }
                        else
                        {
                            if (nextPart.StartsWith(TOKEN_COMMENTS))
                            {
                                //remove comments
                                contents = firstPart + nextPart.Substring(TOKEN_COMMENTS.Length);
                                hasChange = true;
                                i = i - TOKEN_COMMENTS.Length + TOKEN_HEADER_FIRST.Length;
                                continue;
                            }
                        }
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
        static void Main(string[] args)
        {
            if (args.Length == 0)
            {
                Console.Error.WriteLine("Missing argument!");
                return;
            }

            switch (args[0])
            {
                case "4.15":
                    _sDowngrade = true;
                    break;
                case "4.16":
                    _sDowngrade = false;
                    break;
                default:
                    Console.Error.WriteLine("Unsupported version!");
                    return;
            }

            DirectoryInfo di = new DirectoryInfo(Directory.GetCurrentDirectory());
            string targetDir = string.Format("{0}\\Source", di.FullName);
            Console.WriteLine("Processing for {0}... {1}", _sDowngrade ? "downgrade" : "upgrade", targetDir);
            ProcessFolder(new DirectoryInfo(targetDir));
            targetDir = string.Format("{0}\\Plugins", di.FullName);
            ProcessFolder(new DirectoryInfo(targetDir));
        }
    }
}
