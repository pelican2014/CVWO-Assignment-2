<?php

namespace CVWO\HelloWorld {

	require_once drupal_get_path("module", MODULE_NAME)."/helloworld_d7_api.inc";

	/**
	 * Displays the form for adding/viewing/editing details of the user record
	 */
	function helloworld_import_form($form, &$form_state, $page_mode = 'add', $helloworld_id = null) {
		$form = array();

		if ($helloworld_id != null && $page_mode != 'import') {
			// we store the helloworld_id here for future reference (when updating/deleting later)
			$form['helloworld_id'] = array(
				'#type' => 'hidden',
				'#value' => $helloworld_id,
			);
			if (!load_user($form_state, $helloworld_id)) {
				drupal_set_message('Failed to load user details');
				drupal_goto(MODULE_NAME . "import");
			}
		}
		
		if (isset($form_state['values'])) {
			$values = $form_state['values'];
		} else {
			$values = array(
				'text' => '',
				'comment' => '',
			);
		}
		
		if ($page_mode == 'view') {
			$disabled = TRUE;
		} else {
			$disabled = FALSE;
		}
		
		$form['instructions'] = array(
			'#prefix' => '<div>',
			'#markup' => t('Enter multiple lines of data to mass import user records. Each line consists of a user\'s name \
				followed by their email address. A comma separates the name from the email.'),
			'#suffix' => '</div>',
		);
		
		$form['text'] = array(
			'#type' => 'textarea',
			'#title' => t('User info (name,email)'),
			'#default_value' => $values['text'],
			'#disabled' => $disabled,
			'#size' => 64,
			'#required' => TRUE,
		);
		
		$form['comment'] = array(
			'#type' => 'textarea',
			'#title' => t('Comments'),
			'#disabled' => $disabled,
			'#default_value' => $values['comment'],
			'#rows' => 3,
			'#required' => FALSE,
		);

		// Submit button
		$form['add'] = array(
			'#type' => 'submit',
			'#value' => t('Add'),
		);
	  
		return $form;
	}

	// TO DO: change import_form_validate and import_form_submit to process multiple lines of user data

	/**
	 * Implements import_form_validate
	 */
	function helloworld_import_form_validate($form, &$form_state){
		// valid_email_address is from drupal api
		// validation function checks whether the email is valid before inserting/updating/deleting
		if (!valid_email_address($form_state['values']['email'])){
			form_set_error('email', t('Please enter a valid email address.'));
		}
	}

	/**
	 * Implements import_form_submit
	 */
	function helloworld_import_form_submit($form, &$form_state){
		$button_name = $form_state['values']['op'];
		$error = FALSE;

		switch($button_name) {
			case 'Add': 
				if (add_user($form_state['values'])) {
					drupal_set_message(t('%name has been successfully added', array('%name' => $form_state['values']['name'])));
				} else {
					drupal_set_message(t('An error was encountered while adding %name', array('%name' => $form_state['values']['name'])));
					$error = TRUE;
				}
				break;
		}
		
		// if we encountered an error, we keep the form as it is
		// else we redirect back to home page
		if ($error) {
			$form_state['rebuild'] = TRUE;
		} else {
			$form_state['redirect'] = MODULE_NAME;
		}
	}
}
