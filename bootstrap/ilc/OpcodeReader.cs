using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace ilc
{
    public class OpcodeReader
    {
        private Stream _stream;

        public OpcodeReader(Stream stream)
        {
            _stream = stream;
        }

        public byte ReadU8()
        {
            return (byte) _stream.ReadByte();
        }

        public ushort ReadU16()
        {
            return (ushort) (((ReadU8() << 8) | ReadU8()) >> 0);
        }


        public uint ReadU32()
        {
            return (uint) ((ReadU8() << 24) |
                           (ReadU8() << 16) |
                           (ReadU8() << 8) |
                           (ReadU8()));
        }

        public sbyte ReadI8()
        {
            return (sbyte) ReadU8();
        }

        public short ReadI16()
        {
            return (short) (((ReadU8() << 8) | ReadU8()) >> 0);
        }


        public int ReadI32()
        {
            return ((ReadU8() << 24) |
                    (ReadU8() << 16) |
                    (ReadU8() << 8) |
                    (ReadU8()));
        }

        public uint ReadU64()
        {
            return (uint) ((ReadU8() << 56) |
                           (ReadU8() << 48) |
                           (ReadU8() << 40) |
                           (ReadU8() << 32) |
                           (ReadU8() << 24) |
                           (ReadU8() << 16) |
                           (ReadU8() << 8) |
                           (ReadU8()));
        }

        public int ReadI64()
        {
            return ((ReadU8() << 56) |
                    (ReadU8() << 48) |
                    (ReadU8() << 40) |
                    (ReadU8() << 32) |
                    (ReadU8() << 24) |
                    (ReadU8() << 16) |
                    (ReadU8() << 8) |
                    (ReadU8()));
        }

        public float ReadF32()
        {
            return ((ReadU8() << 24) |
                    (ReadU8() << 16) |
                    (ReadU8() << 8) |
                    (ReadU8()));
        }

        public string ReadStr()
        {
            var leng = ReadU32();
            var sb = new StringBuilder();

            for (int i = 0; i < leng; i++)
            {
                sb.Append((char) ReadU8());
            }

            return sb.ToString();
        }


        public string[] ReadStrArray()
        {
            var re = new List<string>();

            var leng = ReadU32();
            for (int i = 0; i < leng; i++)
            {
                re.Add(ReadStr());
            }

            return re.ToArray();
        }

        public Opcode ReadOpcode()
        {
            var id = ReadU8();


            switch ((OpcodeType) id)
            {
                case OpcodeType.Fcall:
                    return new Opcode()
                    {
                        Id = (OpcodeType) id,
                        A0 = ReadStr(),
                        A1 = ReadStrArray()
                    };
                case OpcodeType.Invoke:
                    return new Opcode()
                    {
                        Id = (OpcodeType) id,
                        A0 =  ReadStrArray()
                    };

                case OpcodeType.Label:
                    return new Opcode()
                    {
                        Id = (OpcodeType) id,
                        A0 = ReadStr(),
                        A1 = ReadStrArray()
                    };
                case OpcodeType.Goto:
                    return new Opcode()
                    {
                        Id = (OpcodeType) id,
                        A0 = ReadStr()
                    };

                case OpcodeType.Jz:
                    return new Opcode()
                    {
                        Id = (OpcodeType) id,
                        A0 = ReadStr()
                    };

                case OpcodeType.PushU8:
                    return new Opcode()
                    {
                        Id = (OpcodeType) id,
                        A0 = ReadU8()
                    };
                case OpcodeType.PushU16:
                    return new Opcode()
                    {
                        Id = (OpcodeType) id,
                        A0 = ReadU16()
                    };
                case OpcodeType.PushU32:
                    return new Opcode()
                    {
                        Id = (OpcodeType) id,
                        A0 = ReadU32()
                    };
                case OpcodeType.PushI8:
                    return new Opcode()
                    {
                        Id = (OpcodeType) id,
                        A0 = ReadI8()
                    };
                case OpcodeType.PushI16:
                    return new Opcode()
                    {
                        Id = (OpcodeType) id,
                        A0 = ReadI16()
                    };
                case OpcodeType.PushI32:
                    return new Opcode()
                    {
                        Id = (OpcodeType) id,
                        A0 = ReadI32()
                    };
                case OpcodeType.PushI64:
                    return new Opcode()
                    {
                        Id = (OpcodeType) id,
                        A0 = ReadI64()
                    };
                case OpcodeType.PushU64:
                    return new Opcode()
                    {
                        Id = (OpcodeType) id,
                        A0 = ReadU64()
                    };
                case OpcodeType.PushF32:
                    return new Opcode()
                    {
                        Id = (OpcodeType) id,
                        A0 = ReadF32()
                    };
                case OpcodeType.PushStr:
                    return new Opcode()
                    {
                        Id = (OpcodeType) id,
                        A0 = ReadStr()
                    };
                case OpcodeType.PushFn:
                    return new Opcode()
                    {
                        Id = (OpcodeType) id,
                        A0 = ReadStr()
                    };
                case OpcodeType.LoadLoc:
                case OpcodeType.AddressArg:
                case OpcodeType.AddressLoc:
                case OpcodeType.StoreLoc:
                case OpcodeType.LoadArg:
                case OpcodeType.StoreArg:
                    return new Opcode()
                    {
                        Id = (OpcodeType) id,
                        A0 = ReadU32()
                    };
                default:
                    return new Opcode() {Id = (OpcodeType) id};
            }

            return null;
        }

        public List<Opcode> ReadAll()
        {
            var re = new List<Opcode>();


            while (_stream.Position < _stream.Length)
            {
                re.Add(ReadOpcode());
            }

            return re;
        }
    }
}