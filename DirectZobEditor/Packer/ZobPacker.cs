using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace DirectZobEditor.Packer
{
    class ZobFilePath
    {
        public string name { get; set; }
        public string absolutePath { get; set; }
        public string relativePath { get; set; }
        public string extension { get; set; }
        public string nameWithoutExt { get; set; }
    }

    class ZobPacker
    {
        private DirectZobEditorForm m_editor;
        private string m_tmpFolder;
        private string m_outFolder;
        private StreamWriter m_log;
        private string m_logFile;
        private readonly string PAK_NAME = "0.dzp";


        public ZobPacker(DirectZobEditorForm f, string outFolder)
        {
            m_editor = f;
            m_tmpFolder = Path.Combine(System.IO.Path.GetTempPath(), "directzob", "pack");
            if(Directory.Exists(m_tmpFolder))
            {
                Directory.Delete(m_tmpFolder, true);
            }
            Directory.CreateDirectory(m_tmpFolder);
            m_outFolder = outFolder;
            m_logFile = Path.Combine(m_outFolder, "pack_log.txt");
            m_log =  new StreamWriter(m_logFile);
        }

        public void AddScene(string path, string file)
        {
            XmlDocument xml = new XmlDocument();
            xml.Load(Path.Combine(path, file));
            XmlNodeList xnListImages = xml.SelectNodes("//Var[@Name='Image']");
            XmlNodeList xnListFiles = xml.SelectNodes("//Var[@Name='File']");
            XmlNodeList xnListFontXml = xml.SelectNodes("//Var[@Name='Font Xml']");
            XmlNodeList xnListFontImage = xml.SelectNodes("//Var[@Name='Font Image']");
            XmlNodeList xnListMesh = xml.SelectNodes("//Var[@Name='Mesh']");

            PackXmls(xnListFontXml, path);
            PackImages(xnListFontImage, path);
            PackImages(xnListImages, path);
            PackModels(xnListFiles, path);
            PackModels(xnListMesh, path);
            xml.Save(Path.Combine(m_tmpFolder, file));
            //Copy(xmlFile,, START_FILENAME);
        }

        public void FinishPacking(string entryLevel)
        {
            StreamWriter s = File.CreateText(Path.Combine(m_tmpFolder, "0"));
            s.Write(entryLevel);
            s.Flush();
            s.Close();
            m_editor.logInfo("Finished packing data to '" + m_tmpFolder + "', zipping");
            string outZip = Path.Combine(m_outFolder, PAK_NAME);
            if (File.Exists(outZip))
            {
                File.Delete(outZip);
            }
            ZipFile.CreateFromDirectory(m_tmpFolder, outZip);
            m_editor.logInfo("Finished compression. Archive available here : '" + outZip + "'");
            m_log.Close();
        }

        private ZobFilePath getZobFilePath(XmlNode n, string path)
        {
            string data = n.Attributes["value"].Value;
            ZobFilePath p = new ZobFilePath();
            string[] d = data.Split(';');
            if (d[1] == "/")
            {
                d[1] = "";
            }
            p.name = d[0];
            if (d[3] == "0") //relative
            {
                p.absolutePath = Path.Combine(path, d[1], d[2]);
                p.relativePath = Path.Combine(d[1], d[2]);
            }
            else
            {
                p.absolutePath = Path.Combine(d[1], d[2]);
                //all files in absolute path are copied to the root
                p.relativePath = d[2];
                n.Attributes["value"].Value = p.name+";/;"+p.relativePath+";0";
                m_editor.logWarning( p.name + " has an absolute path('"+p.absolutePath+"'). Changed to relative to the root of the archive ('" + p.relativePath + "')");
            }
            p.nameWithoutExt = Path.GetFileNameWithoutExtension(p.absolutePath);
            p.extension = Path.GetExtension(p.absolutePath);
            return p;
        }

        private void Copy(string src, string dst, string rel)
        {
            try
            {
                string dstFolder = Path.GetDirectoryName(dst);
                if (!Directory.Exists(dstFolder))
                {
                    Directory.CreateDirectory(dstFolder);
                }
                File.Copy(src, dst, true);
                m_editor.logInfo("packed " + rel);
                m_log.WriteLine(src + " -> " + rel);
            }
            catch(Exception e)
            {
                m_editor.logError("packing error : " + e.Message);
                m_log.WriteLine("ERROR : " + src + " -> " + e.Message);
            }
        }
        private void PackXmls(XmlNodeList l, string p)
        {
            foreach (XmlNode n in l)
            {
                ZobFilePath zfp = getZobFilePath(n, p);
                Copy(zfp.absolutePath, Path.Combine(m_tmpFolder, zfp.relativePath), zfp.relativePath);
            }
        }
        private void PackModels(XmlNodeList l, string p)
        {
            foreach (XmlNode n in l)
            {
                ZobFilePath zfp = getZobFilePath(n, p);
                string outPath = Path.Combine(m_tmpFolder, zfp.relativePath);
                string outFolder = Path.GetDirectoryName(outPath);
                if(!Directory.Exists(outFolder))
                {
                    Directory.CreateDirectory(outFolder);
                }
                if (zfp.extension == ".obj")
                {
                    string mtlName = zfp.nameWithoutExt + ".mtl";
                    string mtl = Path.Combine(Path.GetDirectoryName(zfp.absolutePath), mtlName);
                    if (File.Exists(mtl))
                    {
                        Copy(mtl, Path.Combine(outFolder, mtlName), mtlName);
                        string[] f = File.ReadAllText(mtl).Replace("\r", string.Empty).Replace("\\\\", "/").Replace("\\", "/").Split('\n');
                        foreach(string s in f)
                        {
                           if(s.StartsWith("map_Kd"))
                            {  
                                string texFile = s.Split(' ')[1];
                                //texFile = texFile.Replace("\\", "/");
                                string imgPathIn = Path.Combine(Path.GetDirectoryName(mtl), texFile);
                                string imgPathOut = Path.Combine(outFolder, texFile);
                                Copy(imgPathIn, imgPathOut, texFile);
                            }
                        }
                    }
                }
                Copy(zfp.absolutePath, outPath, zfp.relativePath);
            }
        }
        private void PackImages(XmlNodeList xnListImages, string p)
        {
            foreach( XmlNode n in xnListImages)
            {
                ZobFilePath zfp = getZobFilePath(n, p);
                Copy(zfp.absolutePath, Path.Combine(m_tmpFolder, zfp.relativePath), zfp.relativePath);
            }
        }
    }
}
