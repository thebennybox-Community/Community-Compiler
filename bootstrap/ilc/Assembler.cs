using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using ilc.Nasm;

namespace ilc
{
    public class Assembler
    {
        private List<object> SectionText { get; set; } = new List<object>();
        private List<object> SectionData { get; set; } = new List<object>();


        private void Add(object c) => SectionText.Add(c);

        private Dictionary<string, int> LabelSizes { get; set; } = new Dictionary<string, int>();
        private Dictionary<string, int> LabelLocalSizes { get; set; } = new Dictionary<string, int>();

        private int _gCounter { get; set; } = 0;

        public Assembler(List<Opcode> opcodes)
        {
            var lblBiggest = 0;
            var lblBuffer = "";

            foreach (var opcode in opcodes)
            {
                if (opcode.Id == OpcodeType.Ret)
                {
                }
                else if (opcode.Id == OpcodeType.StoreLoc)
                {
                    var z = (uint) opcode.A0;
                    if (z > lblBiggest) lblBiggest = (int) z;
                }
                else if (opcode.Id == OpcodeType.LoadLoc)
                {
                    var z = (uint) opcode.A0;
                    if (z > lblBiggest) lblBiggest = (int) z;
                }
                else if (opcode.Id == OpcodeType.Label)
                {
                    if (lblBuffer != "")
                    {
                        LabelLocalSizes.Add(lblBuffer, lblBiggest == 0 ? 0 : (lblBiggest / 4) + 1);
                        lblBiggest = 0;
                        lblBuffer = "";
                    }

                    lblBuffer = opcode.A0.ToString();

                    var x = ((string[]) opcode.A1);
                    var total = 0;
                    for (var i = 1; i < x.Length; i++)
                    {
                        var s = x[i];
                        switch (s)
                        {
                            case "str":
                            case "fn":
                                total += 4;
                                break;
                            case "bool":
                                total += 1;
                                break;
                            default:
                                total += int.Parse(s.Remove(0, 1)) / 8;
                                break;
                        }
                    }

                    LabelSizes.Add(opcode.A0.ToString(), total);
                }
            }

            if (lblBuffer != "") LabelLocalSizes.Add(lblBuffer, (lblBiggest / 4));

            foreach (var opcode in opcodes)
            {
                switch (opcode.Id)
                {
                    case OpcodeType.nop:
                        Add(new Nop());
                        break;

                    case OpcodeType.PushFn:
                        if (!LabelSizes.ContainsKey(opcode.A0.ToString()))
                        {
                            Add($"extern {opcode.A0}");
                        }

                        Add($"push {opcode.A0}");
                        break;
                    case OpcodeType.PushU8:
                    case OpcodeType.PushU16:
                    case OpcodeType.PushU32:
                    case OpcodeType.PushU64:
                    case OpcodeType.PushI8:
                    case OpcodeType.PushI16:
                    case OpcodeType.PushI32:
                    case OpcodeType.PushI64:
                        Add(new Push(opcode.A0));
                        break;
                    case OpcodeType.PushF32:
                       // SectionData.Add($"flt{++_gCounter} dd ''");
                        Add($"push __float32__( {((float)opcode.A0):n8} )");
                        break;
                    case OpcodeType.PushF64:
                        Add($"push __float64__( {((double)opcode.A0):n8} )");
                        break;
                        
                    case OpcodeType.PushStr:
                        SectionData.Add($"str{++_gCounter} db '{opcode.A0}',0");
                        Add($"mov eax, str{_gCounter}");
                        Add(new Push(Registers.Eax));
                        break;
                    case OpcodeType.PushTrue:
                        Add(new Push(1));
                        break;
                    case OpcodeType.PushFalse:
                        Add(new Push(0));
                        break;

                    case OpcodeType.LoadLoc:
                        Add(new Mov(Registers.Eax, Registers.Ebp)
                        {
                            SourceOffset = -(4 + (int) (uint) opcode.A0)
                        });

                        Add(new Push(Registers.Eax));
                        break;
                    case OpcodeType.StoreLoc:
                        Add(new Pop(Registers.Eax));
                        Add(new Mov(Registers.Ebp, Registers.Eax)
                        {
                            DestinationOffset = -(4 + (int) (uint) opcode.A0)
                        });
                        break;
                    case OpcodeType.LoadArg:
                        Add(new Mov(Registers.Eax, Registers.Ebp)
                        {
                            SourceOffset = (8 + (int) (uint) opcode.A0)
                        });
                        Add(new Push(Registers.Eax));
                        break;
                    case OpcodeType.StoreArg:
                        Add(new Pop(Registers.Eax));
                        Add(new Mov(Registers.Ebp, Registers.Eax)
                        {
                            DestinationOffset = (8 + (int) (uint) opcode.A0)
                        });
                        break;

                    case OpcodeType.Add:
                        Add(new Pop(Registers.Ecx));
                        Add(new Pop(Registers.Eax));
                        Add(new Add(Registers.Eax, Registers.Ecx));
                        Add(new Push(Registers.Eax));

                        break;
                    case OpcodeType.Sub:
                        Add(new Pop(Registers.Ecx));
                        Add(new Pop(Registers.Eax));
                        Add(new Sub(Registers.Eax, Registers.Ecx));
                        Add(new Push(Registers.Eax));
                        break;
                    case OpcodeType.Mul:
                        Add(new Pop(Registers.Ecx));
                        Add(new Pop(Registers.Eax));
                        Add(new Mul(Registers.Eax, Registers.Ecx));
                        Add(new Push(Registers.Eax));
                        break;
                    case OpcodeType.Div:
                        Add(new Pop(Registers.Ecx));
                        Add(new Pop(Registers.Eax));
                        Add(new Div(Registers.Eax, Registers.Ecx));
                        Add(new Push(Registers.Eax));
                        break;
                    case OpcodeType.Neg:
                        Add(new Pop(Registers.Eax));
                        Add(new Neg(Registers.Eax));
                        Add(new Push(Registers.Eax));
                        break;
                    case OpcodeType.Shl:
                        Add(new Pop(Registers.Ecx));
                        Add(new Pop(Registers.Eax));
                        Add(new Shl(Registers.Eax, Registers.Ecx));
                        Add(new Push(Registers.Eax));
                        break;
                    case OpcodeType.Shr:
                        Add(new Pop(Registers.Ecx));
                        Add(new Pop(Registers.Eax));
                        Add(new Shr(Registers.Eax, Registers.Ecx));
                        Add(new Push(Registers.Eax));
                        break;
                    case OpcodeType.And:
                        Add(new Pop(Registers.Ecx));
                        Add(new Pop(Registers.Eax));
                        Add(new And(Registers.Eax, Registers.Ecx));
                        Add(new Push(Registers.Eax));
                        break;
                    case OpcodeType.Or:
                        Add(new Pop(Registers.Ecx));
                        Add(new Pop(Registers.Eax));
                        Add(new Or(Registers.Eax, Registers.Ecx));
                        Add(new Push(Registers.Eax));
                        break;
                    case OpcodeType.Xor:
                        Add(new Pop(Registers.Ecx));
                        Add(new Pop(Registers.Eax));
                        Add(new Xor(Registers.Eax, Registers.Ecx));
                        Add(new Push(Registers.Eax));
                        break;
                    case OpcodeType.Copy:
                        Add(new Pop(Registers.Eax));
                        Add(new Push(Registers.Eax));
                        Add(new Push(Registers.Eax));
                        break;
                    case OpcodeType.Del:
                        Add(new Pop(Registers.Eax));
                        break;
                    case OpcodeType.Read:
                        Add(new Pop(Registers.Eax));
                        Add(new Mov(Registers.Ecx, Registers.Eax)
                        {
                            SourceOffsetDeRef = true
                        });
                        Add(new Push(Registers.Ecx));
                        break;
                    case OpcodeType.Write:
                        Add(new Pop(Registers.Ecx));
                        Add(new Pop(Registers.Eax));
                        Add(new Mov(Registers.Eax, Registers.Ecx)
                        {
                            DestinationDeRef = true
                        });
                        break;

                    case OpcodeType.Ret:
                        Add(new Ret());
                        break;


                    case OpcodeType.Fcall:
                        if (!LabelSizes.ContainsKey(opcode.A0.ToString()))
                        {
                            Add($"extern {opcode.A0}");
                        }

                        var x = ((string[]) opcode.A1);
                        var tot = 0;

                        if (x != null)
                        {
                            for (var i = 0; i < x.Length; i++)
                            {
                                var s = x[i];
                                switch (s)
                                {
                                    case "str":
                                    case "fn":
                                        tot += 4;
                                        break;
                                    case "bool":
                                        tot += 1;
                                        break;
                                    default:
                                        tot += int.Parse(s.Remove(0, 1)) / 8;
                                        break;
                                }
                            }
                        }

                        // Add($"mov edx, esp");
                        Add($"call {opcode.A0}");

                        if (!LabelSizes.ContainsKey(opcode.A0.ToString()))
                        {
                            Add($"add esp, {tot}");
                        }
                        else
                        {
                            Add($"add esp, {LabelLocalSizes[opcode.A0.ToString()]}");
                        }


                        if (!LabelSizes.ContainsKey(opcode.A0.ToString()))
                        {
                            Add(new Push(Registers.Eax));
                        }


                        break;
                    case OpcodeType.Label:

                        var a = LabelLocalSizes[opcode.A0.ToString()] / 4;
                        if (a % 4 == 0) a++;

                        var total = a * 16;
                        Add($"global {opcode.A0}");
                        Add($"{opcode.A0}:");
                        Add($"push ebp");
                        Add($"mov ebp, esp");
                        Add($"sub esp, {total}");

                        break;
                    case OpcodeType.Goto:
                        Add($"jmp {opcode.A0}");
                        break;

                    case OpcodeType.Jz:
                        Add(new Pop(Registers.Eax));
                        Add($"cmp Eax, 0");
                        Add($"je {opcode.A0}");
                        break;

                    case OpcodeType.Ifeq:
                        Add(new Pop(Registers.Eax));
                        Add($"cmp Eax, 0");
                        Add($"jne branch{++_gCounter}");
                        Add(new Push(1));
                        Add($"branch{_gCounter}:");
                        break;
                    case OpcodeType.Ifgt:
                        Add(new Pop(Registers.Eax));
                        Add($"cmp Eax, 0");
                        Add($"jl branch{++_gCounter}");
                        Add(new Push(1));
                        Add($"branch{_gCounter}:");
                        break;
                    case OpcodeType.Ifst:
                        Add(new Pop(Registers.Eax));
                        Add($"cmp Eax, 0");
                        Add($"jg branch{++_gCounter}");
                        Add(new Push(1));
                        Add($"branch{_gCounter}:");
                        break;

                    default:
                        throw new ArgumentOutOfRangeException();
                }
            }

            Optimize();
        }


        private void Optimize()
        {
            for (var i = 0; i < SectionText.Count - 1; i++)
            {
                var a = SectionText[i];
                var b = SectionText[i + 1];

                //push A, pop A = pointless
                if (a is Push ap && b is Pop bp)
                {
                    if (ap.Value is Registers
                        && bp.Value is Registers
                        && (Registers) ap.Value == (Registers) bp.Value)
                    {
                        SectionText.RemoveAt(i);
                        SectionText.RemoveAt(i);
                    }
                }
            }
        }

        public void WriteNasmAssemblyFile(string path)
        {
            var sb = new StringBuilder();

            sb.AppendLine("global main");
            sb.AppendLine("main:");
            sb.AppendLine("call __start");
            sb.AppendLine("mov eax, 0");
            sb.AppendLine("ret");
            sb.AppendLine("__start:");
            
            foreach (var o in SectionText)
            {
                sb.AppendLine(o.ToString());
            }

            sb.AppendLine("section .data");

            foreach (var o in SectionData)
            {
                sb.AppendLine(o.ToString());
            }


            File.WriteAllText(path, sb.ToString());
        }
    }
}