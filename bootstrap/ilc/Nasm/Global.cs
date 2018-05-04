namespace ilc.Nasm
{
    public class Global
    {
        public Global(object value)
        {
            Value = value;
        }

        public object Value { get; set; }

        public override string ToString()
        {
            return $"global {Value}";
        }
    }
}