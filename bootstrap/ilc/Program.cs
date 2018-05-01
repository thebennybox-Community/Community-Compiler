using System;
using System.IO;

namespace ilc
{
    class Program
    {
        static void Main(string[] args)
        {
            var ops = AssemblyParser.Parser(File.ReadAllText("test.il"));
        }
    }
}