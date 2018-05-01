namespace ilc.Nasm
{
    public class Add
    {
        public Registers Destination { get; set; }
        public Registers Source { get; set; }

        public Add(Registers destination, Registers source)
        {
            Destination = destination;
            Source = source;
        }

        public override string ToString()
        {
            return $"add {Destination}, {Source}";
        }
    }
}