using System.Runtime.InteropServices;

namespace ilc.Nasm
{
    public class Extern
    {
        public Extern(object value)
        {
            Value = value;
        }

        public object Value { get; set; }

        public override string ToString()
        {
            bool isWindows = System.Runtime.InteropServices.RuntimeInformation
                .IsOSPlatform(OSPlatform.Windows);
            return $"extern {(isWindows ? "_" : "")}{Value}";
        }
    }
}