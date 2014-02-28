<?php

/**
 * @file
 * Code for creative commons form elements.
 */

/**
 * Creates a creative_commons form element.
 *
 * @param array $form_state
 *   The form state.
 *
 * @return array
 *   The element definition.
 */
function xml_form_elements_creative_commons($element, &$form_state) {
  $modification_options = array(
    'y' => t('Yes'),
    'n' => t('No'),
    'sa' => t('Yes, as long as others share alike'),
  );

  $countries = array(
    'international' => t('International'),
    'ar' => t('Argentina'),
    'au' => t('Australia'),
    'at' => t('Austria'),
    'be' => t('Belgium'),
    'br' => t('Brazil'),
    'bg' => t('Bulgaria'),
    'ca' => t('Canada'),
    'cl' => t('Chile'),
    'cn' => t('China Mainland'),
    'co' => t('Colombia'),
    'cr' => t('Costa Rica'),
    'hr' => t('Croatia'),
    'cz' => t('Czech Republic'),
    'dk' => t('Denmark'),
    'ec' => t('Ecuador'),
    'eg' => t('Egypt'),
    'ee' => t('Estonia'),
    'fi' => t('Finland'),
    'fr' => t('France'),
    'de' => t('Germany'),
    'gr' => t('Greece'),
    'gt' => t('Guatemala'),
    'hk' => t('Hong kong'),
    'hu' => t('Hungary'),
    'in' => t('India'),
    'ie' => t('Ireland'),
    'il' => t('Israel'),
    'it' => t('Italy'),
    'jp' => t('Japan'),
    'lu' => t('Luxembourg'),
    'mk' => t('Macedonia'),
    'my' => t('Malaysia'),
    'mt' => t('Malta'),
    'mx' => t('Mexico'),
    'nl' => t('Netherlands'),
    'nz' => t('New Zealand'),
    'no' => t('Norway'),
    'pe' => t('Peru'),
    'ph' => t('Philippines'),
    'pl' => t('Poland'),
    'pt' => t('Portugal'),
    'pr' => t('Puerto Rico'),
    'ro' => t('Romania'),
    'rs' => t('Serbia'),
    'sg' => t('Singapore'),
    'si' => t('Slovenia'),
    'za' => t('South Africa'),
    'kr' => t('South Korea'),
    'es' => t('Spain'),
    'se' => t('Sweden'),
    'ch' => t('Switzerland'),
    'tw' => t('Taiwan'),
    'th' => t('Thailand'),
    'uk' => t('UK: England & Wales'),
    'scotland' => t('Uk: Scotland'),
    'ug' => t('Uganda'),
    'us' => t('United states'),
    'vn' => t('Vietnam'),
  );

  $commercial_options = array(
    'y' => t('Yes'),
    'n' => t('No'),
  );

  // Get an ID for ajax.
  if (isset($form_state['storage']['xml_form_elements'][$element['#name']]['license_output_id'])) {
    $license_output_id = $form_state['storage']['xml_form_elements'][$element['#name']]['license_output_id'];
  }
  else {
    $license_output_id = drupal_html_id('license_output');
    $form_state['storage']['xml_form_elements'][$element['#name']]['license_output_id'] = $license_output_id;
  }

  // Figure out license state.
  if (!isset($form_state['input'][$element['#name']]) && isset($element['#default_value'])) {
    // Reversing this array facilitates value_callback mangling by community.
    $default_value_array = array_reverse(explode('/', $element['#default_value']));
    $properties = explode('-', $default_value_array[3]);

    $commercial = 'y';
    $derivatives = 'y';

    foreach ($properties as $property) {
      switch ($property) {
        case 'by':
          break;

        case 'nc':
          $commercial = 'n';
          break;

        case 'nd':
          $derivatives = 'n';
          break;

        case 'sa':
          $derivatives = 'sa';
          break;
      }
    }

    $jurisdiction = empty($default_value_array[1]) ? 'international' : $default_value_array[1];
  }
  else {
    $derivatives = isset($form_state['input'][$element['#name']]['license_fieldset']['allow_modifications']) ? $form_state['input'][$element['#name']]['license_fieldset']['allow_modifications'] : 'y';
    $commercial = isset($form_state['input'][$element['#name']]['license_fieldset']['allow_commercial']) ? $form_state['input'][$element['#name']]['license_fieldset']['allow_commercial'] : 'y';
    $jurisdiction = isset($form_state['input'][$element['#name']]['license_fieldset']['license_jurisdiction']) ? $form_state['input'][$element['#name']]['license_fieldset']['license_jurisdiction'] : 'ca';
  }

  // Form elements.
  $element['license_fieldset'] = array(
    '#type' => 'fieldset',
    '#collapsed' => FALSE,
    '#collapsible' => TRUE,
    '#title' => t('License'),
  );

  $element['license_fieldset']['allow_modifications'] = array(
    '#type' => 'select',
    '#title' => t('Allow modifications of your work?'),
    '#options' => $modification_options,
    '#default_value' => $derivatives,
    '#ajax' => array(
      'wrapper' => $license_output_id,
      'callback' => 'xml_form_elements_creative_commons_ajax',
    ),
  );

  $element['license_fieldset']['allow_commercial'] = array(
    '#type' => 'select',
    '#title' => t('Allow commercial uses of your work?'),
    '#options' => $commercial_options,
    '#default_value' => $commercial,
    '#ajax' => array(
      'wrapper' => $license_output_id,
      'callback' => 'xml_form_elements_creative_commons_ajax',
    ),
  );

  $element['license_fieldset']['license_jurisdiction'] = array(
    '#type' => 'select',
    '#title' => t('License Jurisdiction'),
    '#options' => $countries,
    '#default_value' => $jurisdiction,
    '#ajax' => array(
      'wrapper' => $license_output_id,
      'callback' => 'xml_form_elements_creative_commons_ajax',
    ),
  );
  // Value gets populated if default value is populated.
  if (current_path() == 'system/ajax' || !$element['#value'] ||
    (isset($element['#default_value']) && $element['#value'] == $element['#default_value'])) {
    $element['#tree'] = TRUE;
  }
  else {
    // I win form builder.
    $element['#tree'] = FALSE;
  }
  
  $response = xml_form_elements_get_creative_commons($commercial, $derivatives, $jurisdiction);
  if ($response === FALSE) {
    // Use a default license
    $default = '<?xml version="1.0" encoding="utf-8"?>
    <result>
     <license-uri>http://creativecommons.org/licenses/by-nc/4.0/</license-uri>
     <license-name>Attribution-NonCommercial 4.0 International</license-name>
     <rdf>
       <rdf:RDF xmlns="http://creativecommons.org/ns#" xmlns:dc="http://purl.org/dc/elements/1.1/" xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" xmlns:rdfs="http://www.w3.org/2000/01/rdf-schema#">
        <Work rdf:about="">
         <license rdf:resource="http://creativecommons.org/licenses/by-nc/4.0/"/>
        </Work>

        <License rdf:about="http://creativecommons.org/licenses/by-nc/4.0/">
          <permits rdf:resource="http://creativecommons.org/ns#DerivativeWorks"/>
          <permits rdf:resource="http://creativecommons.org/ns#Distribution"/>
          <permits rdf:resource="http://creativecommons.org/ns#Reproduction"/>
          <requires rdf:resource="http://creativecommons.org/ns#Attribution"/>
          <requires rdf:resource="http://creativecommons.org/ns#Notice"/>
        </License>
       </rdf:RDF>
     </rdf>
     <licenserdf>
      <rdf:RDF xmlns="http://creativecommons.org/ns#" xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#">
       <License rdf:about="http://creativecommons.org/licenses/by-nc/4.0/">
        <permits rdf:resource="http://creativecommons.org/ns#DerivativeWorks"/>
        <permits rdf:resource="http://creativecommons.org/ns#Distribution"/>
        <permits rdf:resource="http://creativecommons.org/ns#Reproduction"/>
        <requires rdf:resource="http://creativecommons.org/ns#Attribution"/>
        <requires rdf:resource="http://creativecommons.org/ns#Notice"/>
       </License>
      </rdf:RDF>
     </licenserdf>
     <html><a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/"><img alt="Creative Commons License" style="border-width:0" src="http://i.creativecommons.org/l/by-nc/4.0/88x31.png"/></a><br/>This <span xmlns:dc="http://purl.org/dc/elements/1.1/" href="http://purl.org/dc/dcmitype/" rel="dc:type">work</span> is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/">Creative Commons Attribution-NonCommercial 4.0 International License</a>.</html>
    </result>';
    $response = simplexml_load_string($default);
  }
  
  $form_state['storage']['xml_form_elements'][$element['#name']]['license_uri'] = (string) $response->{'license-uri'};
  $element['license_fieldset']['license_output'] = array(
    '#type' => 'item',
    '#id' => $license_output_id,
    '#markup' => '<strong>' . t('Selected license:') . '</strong><div>' . $response->html->asXml() . '</div>',
  );

  return $element;
}

/**
 * Ajax callback to render the CC license.
 */
function xml_form_elements_creative_commons_ajax(&$form, &$form_state) {
  $parents = $form_state['triggering_element']['#parents'];
  array_pop($parents);
  $creative_commons_element = drupal_array_get_nested_value($form, $parents);

  return $creative_commons_element['license_output'];
}

/**
 * Ask the creative commons REST API for a license.
 *
 * Left as a function for use by community modules.
 *
 * @param string $commercial
 *   'y' to allow 'n' to disallow.
 * @param string $derivatives
 *   'y' to allow 'n' to disallow.
 * @param string $jurisdiction
 *   Legal jurisdiction code.
 *
 * @return SimpleXMLElement
 *   The return from the REST API.
 *  whikloj (2014-02-28): This dies ALL the time, retrieving with curl and parse later.
 */
function xml_form_elements_get_creative_commons($commercial, $derivatives, $jurisdiction) {
  $rest_url = url(
    'http://api.creativecommons.org/rest/1.5/license/standard/get',
    array(
      'query' => array(
        'commercial' => $commercial,
        'derivatives' => $derivatives,
        'jurisdiction' => $jurisdiction,
      ),
    )
  );
  
  $cu = curl_init();
  curl_setopt($cu,CURLOPT_URL,$rest_url);
  curl_setopt($cu,CURLOPT_HEADER,1);
  curl_setopt($cu,CURLOPT_RETURNTRANSFER,1);
  $sXML = curl_exec($cu);
  list($head, $body) = preg_split("/(\r?\n){2}/",$sXML);
  $body = trim($body);
  if (preg_match('/^HTTP\/1\.1 200/',$head)){
    $response = simplexml_load_string($body);
  } else { 
    $response = FALSE;
  }
  return $response;
}

/**
 * Value callback for creative commons element.
 *
 * Input isn't set in form builder edits.
 *
 * @param array $element
 *   The element.
 * @param array $input
 *   The input of the element.
 * @param array $form_state
 *   The form state.
 *
 * @return string
 *   The license URI.
 */
function xml_form_elements_creative_commons_value_callback(&$element, $input, &$form_state) {
  if (isset($input) && $input && current_path() != 'system/ajax') {
    return $form_state['storage']['xml_form_elements'][$element['#name']]['license_uri'];
  }
}
