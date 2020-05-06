type Signal = number;
type Sequence = Signal[];

/**
 * Preset Sequence of Signals
 */
export default interface Preset {
    /** title of preset */
    title: string;
    /** name of preset */
    name: string;
    /** signal sequence */
    sequence: Sequence;
}
