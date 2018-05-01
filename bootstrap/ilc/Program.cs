using System;
using System.Collections.Generic;
using System.IO;
using CommandLine;

namespace ilc
{
    class Options
    {
        [Value(0)]
        public IEnumerable<string> InputFiles { get; set; }

        [Option('t', "text", Default = false, HelpText = "Is input in text format")]
        public bool IsText { get; set; }
    }

    class Program
    {
        static void Main(string[] args)
        {
            CommandLine.Parser.Default.ParseArguments<Options>(args)
                .WithParsed<Options>(opts =>
                {
                    foreach (var file in opts.InputFiles)
                    {
                     
                        
                        var fl = new FileInfo(file);

                        var outPath = Path.Combine(fl.DirectoryName,
                            fl.Name.Remove(fl.Name.Length - fl.Extension.Length) + ".asm");


                        var opcodes = new List<Opcode>();
                        
                        if (opts.IsText)
                        {
                            opcodes.AddRange(AssemblyParser.Parser(File.ReadAllText(file)));
                        }
                        else
                        {
                            using (var stream = File.OpenRead(file))
                            {
                                opcodes.AddRange(new OpcodeReader(stream).ReadAll());
                            }
                        }
                        
                        var assembler = new Assembler(opcodes);
                        
                        assembler.WriteNasmAssemblyFile(outPath);
                    }
                })
                .WithNotParsed<Options>((errs) =>
                {
                    foreach (var error in errs)
                    {
                        Console.WriteLine(error.ToString());
                    }
                });
            // var ops = AssemblyParser.Parser(File.ReadAllText("test.il"));
        }
    }
}