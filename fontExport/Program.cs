using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;


namespace fontExport
{
	class Program
	{
		static void Main(string[] args)
		{
			string path = @"D:\_PERSO\directZob\directZob\resources\font2.png";
			Bitmap b = new Bitmap(path);
			int w = b.Width;
			int h = b.Height;

            string output = "";
            //output += "#pragma once\n#include\"Types.h\"\n\nclass Test\n{\npublic:\n";
            //output += "unit width = " + w + ";\n";
            //output += "uint height = " + h + ";\n";
            //output += "\n const uint data["+w*h+"]={";
			Color white = Color.FromArgb(255, 255, 255, 255);
			for(int y=0; y<h; y++)
			{
                output += "\"";
                for (int x=0; x<w; x++)
				{
                    Color c = b.GetPixel(x, y);
                    if(c == white)
                    {
                        output += "0";
                    }
                    else
                    {
                        output += "1";
                    }
                    //output += "0x"+c.ToArgb().ToString("X") + ", ";
                }
				output += "\"\n";
			}
            //output += "\"";
            //output += "};";
            System.IO.File.WriteAllText(@"D:\_PERSO\directZob\directZob\resources\test_alpha.h", output);
		}
	}
}
