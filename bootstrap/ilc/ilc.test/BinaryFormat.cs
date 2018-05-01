using System;
using System.IO;
using Xunit;

namespace ilc.test
{
    public class BinaryFormat
    {
        private void NoArgumentOpcode(byte id, OpcodeType t)
        {
            var bin = new byte[]
            {
                id
            };

            var toks = new OpcodeReader(new MemoryStream(bin)).ReadAll();

            Assert.Equal(toks[0], new Opcode() {Id = t});
        }

        [Fact]
        public void General()
        {
            var bin = new byte[]
            {
                0x00, 0x00, 0x09, 0, 0, 0, 2, (byte) 'H', (byte) 'i',
            };

            var toks = new OpcodeReader(new MemoryStream(bin)).ReadAll();

            Assert.Equal(toks[0], new Opcode() {Id = OpcodeType.nop});
            Assert.Equal(toks[1], new Opcode() {Id = OpcodeType.nop});
            Assert.Equal(toks[2], new Opcode() {Id = OpcodeType.PushStr, A0 = "Hi"});
        }

        [Fact]
        public void Nop()
        {
            var bin = new byte[]
            {
                0x00
            };

            var toks = new OpcodeReader(new MemoryStream(bin)).ReadAll();

            Assert.Equal(toks[0], new Opcode() {Id = OpcodeType.nop});
        }

        [Fact]
        public void PushU8()
        {
            var bin = new byte[]
            {
                0x01, 10
            };

            var toks = new OpcodeReader(new MemoryStream(bin)).ReadAll();

            Assert.Equal(toks[0], new Opcode()
            {
                Id = OpcodeType.PushU8,
                A0 = (byte) 10
            });
        }

        [Fact]
        public void PushU16()
        {
            var bin = new byte[]
            {
                0x02, 0, 10
            };

            var toks = new OpcodeReader(new MemoryStream(bin)).ReadAll();

            Assert.Equal(toks[0], new Opcode()
            {
                Id = OpcodeType.PushU16,
                A0 = (ushort) 10
            });
        }

        [Fact]
        public void PushU32()
        {
            var bin = new byte[]
            {
                0x03, 0, 0, 0, 10
            };

            var toks = new OpcodeReader(new MemoryStream(bin)).ReadAll();

            Assert.Equal(toks[0], new Opcode()
            {
                Id = OpcodeType.PushU32,
                A0 = (uint) 10
            });
        }

        [Fact]
        public void PushI8()
        {
            var bin = new byte[]
            {
                0x04, 10
            };

            var toks = new OpcodeReader(new MemoryStream(bin)).ReadAll();

            Assert.Equal(toks[0], new Opcode()
            {
                Id = OpcodeType.PushI8,
                A0 = (sbyte) 10
            });
        }

        [Fact]
        public void PushI16()
        {
            var bin = new byte[]
            {
                0x05, 0, 10
            };

            var toks = new OpcodeReader(new MemoryStream(bin)).ReadAll();

            Assert.Equal(toks[0], new Opcode()
            {
                Id = OpcodeType.PushI16,
                A0 = (short) 10
            });
        }

        [Fact]
        public void PushI32()
        {
            var bin = new byte[]
            {
                0x06, 0, 0, 0, 10
            };

            var toks = new OpcodeReader(new MemoryStream(bin)).ReadAll();

            Assert.Equal(toks[0], new Opcode()
            {
                Id = OpcodeType.PushI32,
                A0 = 10
            });
        }

        [Fact]
        public void PushF32()
        {
            var bin = new byte[]
            {
                0x07, 0, 0, 0, 10
            };

            var toks = new OpcodeReader(new MemoryStream(bin)).ReadAll();

            Assert.Equal(toks[0], new Opcode()
            {
                Id = OpcodeType.PushF32,
                A0 = (float) 10
            });
        }

        [Fact]
        public void PushF64()
        {
            Assert.False(true);
        }

        [Fact]
        public void PushStr()
        {
            var bin = new byte[]
            {
                0x09, 0, 0, 0, 2, (byte) 'A', (byte) 'B'
            };

            var toks = new OpcodeReader(new MemoryStream(bin)).ReadAll();

            Assert.Equal(toks[0], new Opcode()
            {
                Id = OpcodeType.PushStr,
                A0 = "AB"
            });
        }

        [Fact]
        public void PushTrue()
        {
            var bin = new byte[]
            {
                0x0A
            };

            var toks = new OpcodeReader(new MemoryStream(bin)).ReadAll();

            Assert.Equal(toks[0], new Opcode() {Id = OpcodeType.PushTrue});
        }

        [Fact]
        public void PushFalse()
        {
            var bin = new byte[]
            {
                0x0B
            };

            var toks = new OpcodeReader(new MemoryStream(bin)).ReadAll();

            Assert.Equal(toks[0], new Opcode() {Id = OpcodeType.PushFalse});
        }


        [Fact]
        public void LoadLoc()
        {
            var bin = new byte[]
            {
                0x0C, 0, 0, 0, 10
            };

            var toks = new OpcodeReader(new MemoryStream(bin)).ReadAll();

            Assert.Equal(toks[0], new Opcode()
            {
                Id = OpcodeType.LoadLoc,
                A0 = (uint) 10
            });
        }

        [Fact]
        public void StoreLoc()
        {
            var bin = new byte[]
            {
                0x0D, 0, 0, 0, 10
            };

            var toks = new OpcodeReader(new MemoryStream(bin)).ReadAll();

            Assert.Equal(toks[0], new Opcode()
            {
                Id = OpcodeType.StoreLoc,
                A0 = (uint) 10
            });
        }

        [Fact]
        public void LoadArg()
        {
            var bin = new byte[]
            {
                0x0E, 0, 0, 0, 10
            };

            var toks = new OpcodeReader(new MemoryStream(bin)).ReadAll();

            Assert.Equal(toks[0], new Opcode()
            {
                Id = OpcodeType.LoadArg,
                A0 = (uint) 10
            });
        }

        [Fact]
        public void StoreArg()
        {
            var bin = new byte[]
            {
                0x0F, 0, 0, 0, 10
            };

            var toks = new OpcodeReader(new MemoryStream(bin)).ReadAll();

            Assert.Equal(toks[0], new Opcode()
            {
                Id = OpcodeType.StoreArg,
                A0 = (uint) 10
            });
        }

        [Fact]
        public void Add()
        {
            NoArgumentOpcode(0x10, OpcodeType.Add);
        }

        [Fact]
        public void Sub()
        {
            NoArgumentOpcode(0x11, OpcodeType.Sub);
        }

        [Fact]
        public void Mul()
        {
            NoArgumentOpcode(0x12, OpcodeType.Mul);
        }

        [Fact]
        public void Div()
        {
            NoArgumentOpcode(0x13, OpcodeType.Div);
        }

        [Fact]
        public void Neg()
        {
            NoArgumentOpcode(0x14, OpcodeType.Neg);
        }

        [Fact]
        public void Shl()
        {
            NoArgumentOpcode(0x15, OpcodeType.Shl);
        }

        [Fact]
        public void Shr()
        {
            NoArgumentOpcode(0x16, OpcodeType.Shr);
        }

        [Fact]
        public void And()
        {
            NoArgumentOpcode(0x17, OpcodeType.And);
        }

        [Fact]
        public void Or()
        {
            NoArgumentOpcode(0x18, OpcodeType.Or);
        }

        [Fact]
        public void Xor()
        {
            NoArgumentOpcode(0x19, OpcodeType.Xor);
        }

        [Fact]
        public void Copy()
        {
            NoArgumentOpcode(0x1A, OpcodeType.Copy);
        }

        [Fact]
        public void Read()
        {
            NoArgumentOpcode(0x1B, OpcodeType.Read);
        }

        [Fact]
        public void Write()
        {
            NoArgumentOpcode(0x1C, OpcodeType.Write);
        }

        [Fact]
        public void Ifeq()
        {
            NoArgumentOpcode(0x1D, OpcodeType.Ifeq);
        }

        [Fact]
        public void Ifgt()
        {
            NoArgumentOpcode(0x1E, OpcodeType.Ifgt);
        }

        [Fact]
        public void Ifst()
        {
            NoArgumentOpcode(0x1F, OpcodeType.Ifst);
        }

        [Fact]
        public void Fcall()
        {
            var bin = new byte[]
            {
                0x20, 0, 0, 0, 1, (byte) 'L'
            };

            var toks = new OpcodeReader(new MemoryStream(bin)).ReadAll();

            Assert.Equal(toks[0], new Opcode()
            {
                Id = OpcodeType.Fcall,
                A0 = "L"
            });
        }

        [Fact]
        public void Ret()
        {
            NoArgumentOpcode(0x21, OpcodeType.Ret);
        }

        [Fact]
        public void Label()
        {
            var bin = new byte[]
            {
                0x22, 0, 0, 0, 1, (byte) 'L', 0, 0, 0, 1, 0, 0, 0, 1, (byte) 'A'
            };

            var toks = new OpcodeReader(new MemoryStream(bin)).ReadAll();

            Assert.Equal(toks[0].Id, OpcodeType.Label);
            Assert.Equal(toks[0].A0, "L");
            Assert.Equal((toks[0].A1 as string[])?[0], "A");
        }

        [Fact]
        public void Goto()
        {
            var bin = new byte[]
            {
                0x23, 0, 0, 0, 1, (byte) 'L'
            };

            var toks = new OpcodeReader(new MemoryStream(bin)).ReadAll();

            Assert.Equal(toks[0], new Opcode()
            {
                Id = OpcodeType.Goto,
                A0 = "L"
            });
        }
        
        [Fact]
        public void Jz()
        {
            var bin = new byte[]
            {
                0x24, 0, 0, 0, 1, (byte) 'L'
            };

            var toks = new OpcodeReader(new MemoryStream(bin)).ReadAll();

            Assert.Equal(toks[0], new Opcode()
            {
                Id = OpcodeType.Jz,
                A0 = "L"
            });
        }
    }
}