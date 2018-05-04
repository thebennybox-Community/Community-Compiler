using System.Linq;

namespace ilc.Nasm
{
    public class Push
    {
        public Push(object value)
        {
            Value = value;
        }

        public object Value { get; set; }

        public override string ToString()
        {
            if (Value is Mov)
            {
                return $"push {Value.ToString().Split(',').Last().Trim()}";
            }
            
            return $"push {Value}";
        }
    }
}