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
            return $"call {Value}";
        }
    }
    
    public class Extern
    {
        public Extern(object value)
        {
            Value = value;
        }

        public object Value { get; set; }

        public override string ToString()
        {
            return $"extern {Value}";
        }
    }
}