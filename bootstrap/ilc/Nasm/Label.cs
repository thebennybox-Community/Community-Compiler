namespace ilc.Nasm
{
    public class Label
    {
        public Label(object value)
        {
            Value = value;
        }

        public object Value { get; set; }

        public override string ToString()
        {
            return $"{Value}:";
        }
    }
}