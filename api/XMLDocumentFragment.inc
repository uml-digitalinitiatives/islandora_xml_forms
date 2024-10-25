<?php

/**
 * @file
 * Contains the XMLDocumentFragment class.
 */

/**
 * Wrapper class for DOMDocumentFragment with additional functionality.
 */
class XMLDocumentFragment {

    /**
     * The underlying DOMDocumentFragment object.
     *
     * @var DOMDocumentFragment
     */
    private DOMDocumentFragment $fragment;

    /**
     * Constructor for XMLDocumentFragment.
     *
     * @param DOMDocument $document
     *   The DOMDocument to create the fragment for.
     */
    public function __construct(DOMDocument $document) {
        $this->fragment = $document->createDocumentFragment();
    }

    /**
     * Append XML to the fragment.
     *
     * @param string $xml
     *   The XML string to append.
     *
     * @return bool
     *   Returns TRUE on success or FALSE on failure.
     */
    public function appendXML(string $xml): bool {
        return $this->fragment->appendXML($xml);
    }

    /**
     * Get the underlying DOMDocumentFragment.
     *
     * @return DOMDocumentFragment
     *   The underlying DOMDocumentFragment object.
     */
    public function getDOMDocumentFragment(): DOMDocumentFragment {
        return $this->fragment;
    }

    /**
     * Serialize the object.
     *
     * @return array
     *   The serialized data.
     */
    public function __serialize(): array {
        return [
            'fragment' => $this->fragment->ownerDocument->saveXML($this->fragment),
        ];
    }

    /**
     * Unserialize the object.
     *
     * @param array $data
     *   The serialized data.
     */
    public function __unserialize(array $data): void {
        $doc = new DOMDocument();
        $doc->loadXML($data['fragment']);
        $this->fragment = $doc->createDocumentFragment();
        foreach ($doc->childNodes as $child) {
            $this->fragment->appendChild($doc->importNode($child, TRUE));
        }
    }
}
