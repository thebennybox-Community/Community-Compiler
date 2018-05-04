using System.Runtime.InteropServices;

namespace ilc.Nasm
{
    public class Call
    {
        public Call(object value)
        {
            Value = value;
        }

        public object Value { get; set; }

        public override string ToString()
        {
            bool isWindows = System.Runtime.InteropServices.RuntimeInformation
                .IsOSPlatform(OSPlatform.Windows);
            return $"call {(isWindows ? "_" : "")}{Value}";
        }
    }
}