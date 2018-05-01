
namespace ilc.Nasm
{
    public class Pop
    {
        public Pop(object value)
        {
            Value = value;
        }

        public object Value { get; set; }

        public override string ToString()
        {
            if (Value is string)
            {
                return $"pop \"{Value}\"";
            }
            
            return $"pop {Value}";
        }
    }
}