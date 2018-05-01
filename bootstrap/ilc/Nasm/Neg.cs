namespace ilc.Nasm
{
    public class Neg
    {
        public Registers Source { get; set; }

        public Neg(Registers source)
        {
            Source = source;
        }

        public override string ToString()
        {
            return $"neg {Source}";
        }
    }
}