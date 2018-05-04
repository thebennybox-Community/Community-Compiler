namespace ilc.Nasm
{
    public class Lea
    {
        public Registers Destination { get; set; }
        public object Source { get; set; }

        public int DestinationOffset { get; set; }
        public int SourceOffset { get; set; }
        
        public bool DestinationDeRef { get; set; }
        public bool SourceOffsetDeRef { get; set; }

        public Lea(Registers destination, object source)
        {
            Destination = destination;
            Source = source;
        }

        public override string ToString()
        {
            if (DestinationOffset != 0 || DestinationDeRef)
            {
                return $"lea [{Destination}{(DestinationOffset < 0 ? "" : "+") + DestinationOffset}], {Source}";
            }

            if (SourceOffset != 0 || SourceOffsetDeRef)
            {
                return $"lea {Destination}, [{Source}{(SourceOffset < 0 ? "" : "+") + SourceOffset}]";
            }

            return $"lea {Destination}, {Source}";
        }
    }
}