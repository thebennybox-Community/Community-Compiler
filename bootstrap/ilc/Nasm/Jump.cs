namespace ilc.Nasm
{
    public class Jump
    {
        public Jump(object value)
        {
            Value = value;
        }

        public object Value { get; set; }

        public override string ToString()
        {
            return $"jmp {Value}";
        }
    }
}