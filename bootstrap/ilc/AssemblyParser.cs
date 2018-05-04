using System;
using System.Collections.Generic;
using System.Dynamic;
using System.Globalization;

namespace ilc
{
    public static class AssemblyParser
    {
        public static List<Opcode> Parser(string raw)
        {
            var re = new List<Opcode>();

            foreach (var s in raw.Replace("\r\n", "\n").Split('\n'))
            {
                var x = s.Split(';')[0];
                if (string.IsNullOrEmpty(x.Trim())) continue;

                var tmp = new Opcode();

                var segs = x.Trim().Split(' ');

                if (x.Contains("\""))
                {
                    segs = new[]
                    {
                        segs[0],
                        x.Remove(0, segs[0].Length).Trim()
                    };

                }

                
                var fmt = new NumberFormatInfo();
                fmt.NegativeSign = "-";
                fmt.NumberDecimalSeparator = ".";
                
                
                switch (segs[0].Trim().ToLower())
                {
                    case "nop":
                        tmp.Id = OpcodeType.nop;
                        break;
                    case "push.u8":
                        tmp.Id = OpcodeType.PushU8;
                        tmp.A0 = byte.Parse(segs[1]);
                        break;
                    case "push.u16":
                        tmp.Id = OpcodeType.PushU16;
                        tmp.A0 = short.Parse(segs[1]);
                        break;
                    case "push.u32":
                        tmp.Id = OpcodeType.PushU32;
                        tmp.A0 = uint.Parse(segs[1]);
                        break;
                    case "push.i8":
                        tmp.Id = OpcodeType.PushI8;
                        tmp.A0 = sbyte.Parse(segs[1]);
                        break;
                    case "push.i16":
                        tmp.Id = OpcodeType.PushI16;
                        tmp.A0 = short.Parse(segs[1]);
                        break;
                    case "push.i32":
                        tmp.Id = OpcodeType.PushI32;
                        tmp.A0 = int.Parse(segs[1]);
                        break;
                    case "push.i64":
                        tmp.Id = OpcodeType.PushI64;
                        tmp.A0 = long.Parse(segs[1]);
                        break;
                    case "push.u64":
                        tmp.Id = OpcodeType.PushU64;
                        tmp.A0 = ulong.Parse(segs[1]);
                        break;
                    case "push.f32":
                        tmp.Id = OpcodeType.PushF32;
                        tmp.A0 = float.Parse(segs[1], fmt);
                        break;
                    case "push.f64":
                        tmp.Id = OpcodeType.PushF64;
                        tmp.A0 = double.Parse(segs[1], fmt);
                        break;
                    case "push.str":
                        tmp.Id = OpcodeType.PushStr;
                        tmp.A0 = segs[1];
                        break;
                    case "push.fn":
                        tmp.Id = OpcodeType.PushFn;
                        tmp.A0 = segs[1];
                        break;
                    case "push.true":
                        tmp.Id = OpcodeType.PushTrue;
                        break;
                    case "push.false":
                        tmp.Id = OpcodeType.PushFalse;
                        break;
                    case "load.loc":
                        tmp.Id = OpcodeType.LoadLoc;
                        tmp.A0 = uint.Parse(segs[1]);
                        break;
                    case "addr.loc":
                        tmp.Id = OpcodeType.AddressLoc;
                        tmp.A0 = uint.Parse(segs[1]);
                        break;
                    case "addr.arg":
                        tmp.Id = OpcodeType.AddressArg;
                        tmp.A0 = uint.Parse(segs[1]);
                        break;
                    case "store.loc":
                        tmp.Id = OpcodeType.StoreLoc;
                        tmp.A0 = uint.Parse(segs[1]);
                        break;
                    case "load.arg":
                        tmp.Id = OpcodeType.LoadArg;
                        tmp.A0 = uint.Parse(segs[1]);
                        break;
                    case "store.arg":
                        tmp.Id = OpcodeType.StoreArg;
                        tmp.A0 = uint.Parse(segs[1]);
                        break;
                    case "add":
                        tmp.Id = OpcodeType.Add;
                        break;
                    case "del":
                        tmp.Id = OpcodeType.Del;
                        break;
                    case "sub":
                        tmp.Id = OpcodeType.Sub;
                        break;
                    case "mul":
                        tmp.Id = OpcodeType.Mul;
                        break;
                    case "div":
                        tmp.Id = OpcodeType.Div;
                        break;
                    case "neg":
                        tmp.Id = OpcodeType.Neg;
                        break;
                    case "shl":
                        tmp.Id = OpcodeType.Shl;
                        break;
                    case "shr":
                        tmp.Id = OpcodeType.Shr;
                        break;
                    case "and":
                        tmp.Id = OpcodeType.And;
                        break;
                    case "or":
                        tmp.Id = OpcodeType.Or;
                        break;
                    case "xor":
                        tmp.Id = OpcodeType.Xor;
                        break;
                    case "copy":
                        tmp.Id = OpcodeType.Copy;
                        break;
                    case "read":
                        tmp.Id = OpcodeType.Read;
                        break;
                    case "write":
                        tmp.Id = OpcodeType.Write;
                        break;
                    case "ifeq":
                        tmp.Id = OpcodeType.Ifeq;
                        break;
                    case "ifgt":
                        tmp.Id = OpcodeType.Ifgt;
                        break;
                    case "ifst":
                        tmp.Id = OpcodeType.Ifst;
                        break;
                    case "fcall":
                        tmp.Id = OpcodeType.Fcall;
                        tmp.A0 = segs[1];
                        {
                            tmp.A1 = new string[segs.Length - 2];
    
                            var z = tmp.A1 as string[];
    
                            for (int i = 2; i < segs.Length; i++)
                            {
                                z[i - 2] = segs[i];
                            }
                        }
                        break;
                    case "invoke":
                        tmp.Id = OpcodeType.Invoke;
                    {
                        tmp.A0 = new string[segs.Length - 2];
    
                        var z = tmp.A1 as string[];
    
                        for (int i = 2; i < segs.Length; i++)
                        {
                            z[i - 2] = segs[i];
                        }
                    }
                        break;
                    case "ret":
                        tmp.Id = OpcodeType.Ret;
                        break;
                    case "goto":
                        tmp.Id = OpcodeType.Goto;
                        tmp.A0 = segs[1];
                        break;
                    case "jz":
                        tmp.Id = OpcodeType.Jz;
                        tmp.A0 = segs[1];
                        break;

                    default:
                        if (segs[0].Trim().EndsWith(":"))
                        {
                            tmp.Id = OpcodeType.Label;
                            tmp.A0 = segs[0].Trim().Split(':')[0];
                            tmp.A1 = new string[segs.Length - 1];

                            var z = tmp.A1 as string[];

                            for (int i = 1; i < segs.Length; i++)
                            {
                                z[i - 1] = segs[i];
                            }
                        }
                        else
                        {
                            Console.WriteLine($"Error: {segs[0]} is a fictunal construct you made up fool.");
                        }

                        break;
                }

                re.Add(tmp);
            }


            return re;
        }
    }
}