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
            if (Value is string)
            {
               // return $"push \"{Value}\"";
            }
            
            return $"push {Value}";
        }
    }
}