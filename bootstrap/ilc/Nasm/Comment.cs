namespace ilc.Nasm
{
    public class Comment
    {
        public Comment(object value)
        {
            Value = value;
        }

        public object Value { get; set; }

        public override string ToString()
        {
            return $";{Value}";
        }
    }
}