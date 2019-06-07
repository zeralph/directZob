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
			string path = @"C:\Users\zeral\Desktop\Project1\fontExport\fontExport\test_alpha.png";
			Bitmap b = new Bitmap(path);
			int w = b.Width;
			int h = b.Height;

            string output = "#pragma once\n#include\"Types.h\"\n\nclass Test\n{\npublic:\n";
            output += "unit width = " + w + ";\n";
            output += "uint height = " + h + ";\n";
            output += "\n const uint data["+w*h+"]={";
			Color white = Color.FromArgb(255, 255, 255, 255);
			for(int y=0; y<h; y++)
			{
				for(int x=0; x<w; x++)
				{
                    Color c = b.GetPixel(x, y);
                    output += "0x"+c.ToArgb().ToString("X") + ", ";
                }
				output += "\n";
			}
			output += "};";
			System.IO.File.WriteAllText(@"C:\Users\zeral\Desktop\Project1\fontExport\fontExport\test_alpha.h", output);
		}
	}
}
